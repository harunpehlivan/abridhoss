#include "mem.h"
uintptr_t current_break;
int has_initialized = 0;
void *managed_memory_start;
void *last_valid_address;
typedef struct mem_control_block {
  int is_available;
  int size;
} mem_control_block;
mem_control_block pmcb; 
//#define asizeof(x) ((&x + 1) - &x)
#define asizeof(x) ((char *)(&x + 1) - (char *)&x)

	
unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count)
{
  int i;
  for (i=0; i<count;i++) dest[i]=src[i];
  return dest;
}

unsigned char *memset(unsigned char *dest, unsigned char val, int count)
{
  int i;
  for (i=0; i<count;i++) dest[i]=val;
  return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, int count)
{
  int i;
  for (i=0; i<count;i++) dest[i]=val;
  return dest;
}
void *sbrk(intptr_t incr)
{
    uintptr_t old_break = current_break;
    current_break += incr;
    return (void*) old_break;
}

void malloc_init() { /* grab the last valid address from the OS*/
  last_valid_address = sbrk(0);
  /* we don't have any memory to manage yet, so
   *just set the beginning to be last_valid_address */
  managed_memory_start = last_valid_address;
  /* Okay, we're initialized and ready to go */
  has_initialized = 1;
}
void free(void *firstbyte) {
  mem_control_block *mcb;
  mcb = firstbyte - asizeof(pmcb);
  mcb->is_available = 1;
  return;
}
void *malloc(long numbytes) {
  /* Holds where we are looking in memory */
  void *current_location;
  /* This is the same as current_location, but cast to a * memory_control_block
   */
  mem_control_block *current_location_mcb;
  /* This is the memory location we will return.
     It will * be set to 0 until we find something suitable */
  void *memory_location;
  /* Initialize if we haven't already done so */
  if (!has_initialized) {
    malloc_init();
  }
  /* The memory we search for has to include the memory
   * control block, but the user of malloc doesn't need
   * to know this, so we'll just add it in for them. */
  numbytes = numbytes + asizeof(pmcb);
  /* Set memory_location to 0 until we find a suitable * location */
  memory_location = 0;
  /* Begin searching at the start of managed memory */
  current_location = managed_memory_start;
  /* Keep going until we have searched all allocated space */
  while (current_location != last_valid_address) {
    /* current_location and current_location_mcb point
     * to the same address.  However, current_location_mcb
     * is of the correct type so we can use it as a struct.
     * current_location is a void pointer so we can use it
     * to calculate addresses.
     */
    current_location_mcb = (mem_control_block *)current_location;
    if (current_location_mcb->is_available) {
      if (current_location_mcb->size >= numbytes) {
        /* Woohoo!  We've found an open, * appropriately-size location.  */
        /* It is no longer available */
        current_location_mcb->is_available = 0;
        /* We own it */
        memory_location = current_location;
        /* Leave the loop */
        break;
      }
    }
    /* If we made it here, it's because the Current memory
     * block not suitable, move to the next one */
    current_location = current_location + current_location_mcb->size;
  }
  /* If we still don't have a valid location, we'll
   * have to ask the operating system for more memory */
  if (!memory_location) {
    /* Move the program break numbytes further */
    sbrk(numbytes);
    /* The new memory will be where the last valid * address left off */
    memory_location = last_valid_address;
    /* We'll move the last valid address forward * numbytes */
    last_valid_address = last_valid_address + numbytes;
    /* We need to initialize the mem_control_block */
    current_location_mcb = memory_location;
    current_location_mcb->is_available = 0;
    current_location_mcb->size = numbytes;
  }
  /* Now, no matter what (well, except for error conditions),
   * memory_location has the address of the memory, including
   * the mem_control_block */
  /* Move the pointer past the mem_control_block */
  memory_location = memory_location + asizeof(pmcb);
  /* Return the pointer */
  return memory_location;
}
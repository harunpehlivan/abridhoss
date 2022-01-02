#ifndef __MEM_H
#define __MEM_H

typedef unsigned long int uintptr_t;
typedef unsigned long int intptr_t;

extern unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count);
extern unsigned char *memset(unsigned char *dest, unsigned char val, int count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);

void *sbrk(intptr_t incr);

void *malloc(long numbytes);
void free(void *firstbyte);
#endif
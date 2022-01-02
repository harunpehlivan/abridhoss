#include <screen.h>
#include <kshell.h>
#include <timer.h>
#include <ports.h>
#include "./lib/string.h"
#include <stdio.h>
#define NULL ((char *)0)
#define sizeof(type) (char *)(&type+1)-(char*)(&type)

extern int enable_shell;

int cf = 1;
int dfs = 1;
void kshell_init(){
	kprintf("Enabling kernel shell...");
	enable_shell = 1;
	putstr("[DONE]\n", COLOR_GRN, COLOR_BLK);
}

char *username = "root";
char *errorcode = "0";
void kshell(char s[], int i){
	char* start = "";
	char* two = "";
  	strcpy(start, s);
	strcpy(two, s);
  	kprintf("\n");
  	char* cmdnamee = strtok(start, " ");
  if(strcmp(s, "clear")==0){
  	clear_scr();
  } else if(strcmp(s, "help")==0){
  	available_commands();
  } else if(strcmp(s, "about")==0){
  	kprintf("Abrid-HOS-x86: a 32 bit operating system written in C and Assembly\n");
  } else if(strcmp(s, "hlt")==0){
  	halt();
  } else if(strcmp(s, "reboot")==0){
  	reboot();
  } else if(strcmp(s, "unknownerror")==0){
    	kprintf("Printing unknown error:\n");
  	unknown_command(s);
  } else if(strcmp(cmdnamee, "sec")==0) {
  	char *buf;
  	char *str = itoa(get_tracked_seconds(), buf, 10);
  	strcat(str, "\n");
  	kprintf(str);
  } else if(strcmp(cmdnamee, "echo")==0) {
	kprintf(strremove(s,"echo "));
  } else if(strcmp(cmdnamee, "getc")==0) {
	  char *buf;
	kprintf(itoa(gets(), buf, 10));
  } else {
  	unknown_command(s);
  }

  for(int j=0; s[j]!='\0'; j++){
  	s[j]='\0';
  }
  // strcat(strcat(strcat(strcat("[", username), "("), errorcode), ")]$")
  kprintf("[root]$");
}

void halt(){
  	kprintf("Halting system...");
  	putstr("[DONE]", COLOR_GRN, COLOR_BLK);
  	asm volatile("hlt");
}

void reboot(){
  	kprintf("Rebooting...\n");
  	//using 8042 keyboard controller to pulse the CPU's RESET Pin
  	unsigned char temp = 0x02;
  	while(temp & 0x02)
  		temp = in_port_byte(0x64);
  	out_port_byte(0x64, 0xFE);
  	asm volatile("hlt");	
}

void clear_scr(){
	clear(COLOR_WHT, COLOR_BLK);
	set_cursor_pos(0,0);
}

void available_commands(){
    kprintf("Available commands:");
  	putstr("\n\thelp\tclear\tabout\n\thlt\treboot\techo <msg>\n", COLOR_GRN, COLOR_BLK);
}

void unknown_command(char s[]){
	kprintf(s);
  	putstr(" : command not found!\n", COLOR_RED, COLOR_BLK);
  	putstr("Type ", COLOR_GRY, COLOR_BLK);
  	putstr("\"help\" ", COLOR_YEL, COLOR_BLK);
  	putstr("for the list of available commands.\n", COLOR_GRY, COLOR_BLK);
}

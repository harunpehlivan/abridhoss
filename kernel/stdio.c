#include <keyboard.h>
#include <stdio.h>
#include <ports.h>
#include <screen.h>
#include "./lib/string.h"

char gets(){
	return buffer[strlen(buffer)];
}
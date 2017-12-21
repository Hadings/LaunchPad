#include"TM4C123GH6PM.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#define BUFFER_SIZE 100
extern char strBuffer[];
void init_serial(void);
char readChar(void);
void printChar(char c);
void printInt(int n);
void printString(char *);
void readString( char delimiter );

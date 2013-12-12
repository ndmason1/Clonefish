/***********************************************************

util.h 

Declarations and preprocessing for util.cpp.

************************************************************/

#ifndef UTIL_H
#define UTIL_H

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int getch();
string getpass(const char *prompt, bool show_asterisk=true);
bool fileExists(const string& name);

#endif // UTIL_H

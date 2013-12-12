/***********************************************************

cipher.h

Declarations and preprocessing for cipher.cpp.

************************************************************/

#ifndef CIPHER_H
#define CIPHER_H

#include <fstream>
#include <cstdlib>
#include <cstring> 
#include <openssl/md5.h>

#define N 16        // the number of rounds
#define BLOCK_SIZE 2 * sizeof(long)
#define MAX_KEY_LEN 56

using namespace std;

void keySchedule(unsigned char *key); //, int keyLen);
void encryptFile(ifstream* inFile, ofstream* outFile, char *key);
void decryptFile(ifstream* inFile, ofstream* outFile, char *key);
void encryptBlock(unsigned long *L, unsigned long *R);
void decryptBlock(unsigned long * L, unsigned long * R);


#endif // CIPHER_H

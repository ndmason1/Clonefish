/***********************************************************

cipher.cpp 

This file contains functions that implement each part of 
the Blowfish cipher.

************************************************************/

#include "cipher.h"
#include "subkeys.h"

/*
	Performs encryption on an input file using the key provided 
	and writes the encrypted data to an output file.
*/
void encryptFile(ifstream *inFile, ofstream *outFile, char *key)
{
	// run key schedule to set up subkeys
	keySchedule((unsigned char*)key);
	
	// produce an MD5 hash of the key to use as an initialization vector
    unsigned long digest[MD5_DIGEST_LENGTH];    
	MD5((unsigned char*)key, strlen(key), (unsigned char*)digest);
	unsigned long prev_lblock = digest[0];
	unsigned long prev_rblock = digest[1];
	
	// get file size (assumes file opened at end), and start reading from beginning
	unsigned long fSize = inFile->tellg();    
    inFile->seekg(0, inFile->beg);
            
	while (fSize > 0) // encrypt blocks until the file has been read entirely
	{
	    unsigned long lblock;
	    unsigned long rblock;
	    
	    if (fSize < BLOCK_SIZE)  // file length is not a multiple of the block size; need to add padding
	    {	
	        lblock = rblock = 0UL;
	        unsigned long padBytes = BLOCK_SIZE - fSize;
	        
	        if (fSize < sizeof(long)) // less than half a block of plaintext left
	        {	
	            inFile->read(reinterpret_cast< char* >(&lblock),fSize);
	        }
	        else
	        {	         
	            inFile->read(reinterpret_cast< char* >(&lblock),sizeof(long));
	            inFile->read(reinterpret_cast< char* >(&rblock),fSize - sizeof(long));	            
	        }	        
	        
	        // XOR block as per CBC mode
	        lblock ^= prev_lblock;
	    	rblock ^= prev_rblock;
	    	
	    	// encrypt final data block with zero padding	        
	        encryptBlock(&lblock, &rblock);
	        prev_lblock = lblock;
	    	prev_rblock = rblock;	        
            outFile->write( (char*)&lblock, sizeof(long));
            outFile->write( (char*)&rblock, sizeof(long));
	        
	        // add extra block with padding length
	        lblock = rblock = 0UL;
            rblock |= padBytes; // right half is encoded with padding length
            
            // XOR block as per CBC mode
            lblock ^= prev_lblock;
	    	rblock ^= prev_rblock;
	    	
            encryptBlock(&lblock, &rblock);            
            outFile->write( (char*)&lblock, sizeof(long));
            outFile->write( (char*)&rblock, sizeof(long));
	        break;	        
	    }
	    else if (fSize == BLOCK_SIZE)
	    {
	    	// encrypt final data block
	        inFile->read(reinterpret_cast< char* >(&lblock),sizeof(long));
	        inFile->read(reinterpret_cast< char* >(&rblock),sizeof(long));	        
	        	        
	        // XOR block as per CBC mode
	        lblock ^= prev_lblock;
	    	rblock ^= prev_rblock; 
	    	
	        encryptBlock(&lblock, &rblock);
	        prev_lblock = lblock;
	    	prev_rblock = rblock;
            outFile->write( (char*)&lblock, sizeof(long));
            outFile->write( (char*)&rblock, sizeof(long));
	        
	        // add extra block to indicate there was no padding added
            lblock = rblock = 0UL;
            
            // XOR block as per CBC mode            
            lblock ^= prev_lblock;
	    	rblock ^= prev_rblock; 
	    	
            encryptBlock(&lblock, &rblock);
            outFile->write( (char*)&lblock, sizeof(long));
            outFile->write( (char*)&rblock, sizeof(long));
	        break;	
	    }
	    
	    // read in a block of data and encrypt
	    inFile->read(reinterpret_cast< char* >(&lblock),sizeof(long));
	    inFile->read(reinterpret_cast< char* >(&rblock),sizeof(long));
	    
	    // XOR block as per CBC mode
	    lblock ^= prev_lblock;
	    rblock ^= prev_rblock;
	    	    
	    encryptBlock(&lblock, &rblock);
	    prev_lblock = lblock;
	    prev_rblock = rblock;
	    
	    // write encrypted block to output file
	    outFile->write( (char*)&lblock, sizeof(long));
	    outFile->write( (char*)&rblock, sizeof(long));
	    
	    fSize -= BLOCK_SIZE;
	}
}


/*
	Performs decryption on the given input file and writes the 
	plaintext to the given output file.
*/
void decryptFile(ifstream *inFile, ofstream *outFile, char *key)
{	
	// run key schedule to set up subkeys
	keySchedule((unsigned char*)key);
	
	// produce an MD5 hash of the key to use as an initialization vector
	unsigned long digest[MD5_DIGEST_LENGTH];    
	MD5((unsigned char*)key, strlen(key), (unsigned char*)digest);
	unsigned long lcipher = digest[0];
	unsigned long rcipher = digest[1];	
	
	// get file size (assumes file opened at end), and start reading from beginning
	unsigned long fSize = inFile->tellg();    
    inFile->seekg(0, inFile->beg);    
    
	while (fSize > 0) // decrypt blocks until the file has been read entirely
	{
	    unsigned long lblock;
	    unsigned long rblock;
	    unsigned long ltemp;
	    unsigned long rtemp;
	    	    
	    if (fSize == 2 * BLOCK_SIZE)  // reached the last 2 blocks
	    {
	        // read last data block
	        inFile->read(reinterpret_cast< char* >(&lblock),sizeof(long));
	        inFile->read(reinterpret_cast< char* >(&rblock),sizeof(long));
	        
	        // store copy of encrypted block
	        ltemp = lblock;
	        rtemp = rblock;	        	        
	        decryptBlock(&lblock, &rblock);
	        
	        // XOR block as per CBC mode
	        lblock ^= lcipher;
    		rblock ^= rcipher;	        
	        
	        // read padding block
	        unsigned long lpad, rpad;	        
	        inFile->read(reinterpret_cast< char* >(&lpad),sizeof(long));
	        inFile->read(reinterpret_cast< char* >(&rpad),sizeof(long));
	        decryptBlock(&lpad, &rpad);
	        
	        // XOR block as per CBC mode
	        lpad ^= ltemp;
	        rpad ^= rtemp;
	        
	        // make sure we only write the data (not the padding) to the output file
	        if (rpad > sizeof(long))
	        {
	            outFile->write( (char*)&lblock, (BLOCK_SIZE - rpad));
	        }
	        else
	        {
	            outFile->write( (char*)&lblock, sizeof(long));
	            if (rpad != sizeof (long))
	                outFile->write( (char*)&rblock, (sizeof(long) - rpad));
	        }	        
	        break;
	    }
	    
	    // read a block of encrypted data and decrypt
	    inFile->read(reinterpret_cast< char* >(&lblock),sizeof(long));
	    inFile->read(reinterpret_cast< char* >(&rblock),sizeof(long));
	    
	    // store copy of encrypted block 
	    ltemp = lblock;
	    rtemp = rblock;	        
	    
	    decryptBlock(&lblock, &rblock);
	    
	    // XOR block as per CBC mode
    	lblock ^= lcipher;
    	rblock ^= rcipher;
	    
	    // update ciphertext block for CBC decryption
	    lcipher = ltemp;
	    rcipher = rtemp;	    
	    
	    // write encrypted block to output file
	    outFile->write( (char*)&lblock, sizeof(long));
	    outFile->write( (char*)&rblock, sizeof(long));
	    
	    fSize -= BLOCK_SIZE;
	}
}

/*
	Runs the Blowfish key schedule algorithm using the subkey arrays P and S
	(declared and initialized in subkeys.h)
*/
void keySchedule(unsigned char *key)
{       
	int i, j, k;
	unsigned long d, dl, dr;
	int keyLen = strlen((char*)key);
	
	// XOR P array with key
	j = 0;
	for (i = 0; i < N+2; ++i) 
	{
		 d = 0UL;
		 for (k = 0; k < 4; ++k) 
		 {
		     d = (d << 8) | key[j];
		     j++;
		     if (j >= keyLen) j = 0;		     
		 }
		 P[i] = P[i]^d;
	}
	
	// encryption step to further randomize P array and randomize S boxes
	dl=dr=0UL;
	for (i = 0; i < N + 2; i += 2) 
	{
		 encryptBlock(&dl, &dr);
		 P[i] = dl;
		 P[i+1] = dr;
	}	
	for (i = 0; i < 4; ++i) 
	{
		 for (j = 0; j < 256; j+=2) 
		 {
			encryptBlock(&dl, &dr);
			S[i][j] = dl;
			S[i][j+1] = dr;
		 }
	}
}

/*
	The round function.
*/
unsigned long f(unsigned long x)
{
   unsigned short a, b, c, d;
   unsigned long y;
   d = x & 0x00FF; x >>= 8;
   c = x & 0x00FF; x >>= 8;
   b = x & 0x00FF; x >>= 8;
   a = x & 0x00FF;
   y = S[0][a] + S[1][b];
   y = y ^ S[2][c];
   y = y + S[3][d];
   return y;
}

/*
	Perform encryption on a 64 bit block (separated into two halves) via Blowfish algorithm.
*/
void encryptBlock(unsigned long *L, unsigned long *R) 
{
	for (int i=0 ; i<N ; ++i)
	{
	       *L ^= P[i];
	       *R ^= f(*L);
		unsigned long temp = *L;
		*L = *R;
		*R = temp;
	}
	unsigned long temp = *L;
	*L = *R;
	*R = temp;
	*R ^= P[N];
	*L ^= P[N+1];
}

/*
	Perform decryption on a 64 bit block (separated into two halves) via Blowfish algorithm.
*/
void decryptBlock(unsigned long *L, unsigned long *R)
{
	for (int i=N+1 ; i > 1 ; --i) {
		*L ^= P[i];
		*R ^= f(*L);
		unsigned long temp = *L;
		*L = *R;
		*R = temp;
	}
	unsigned long temp = *L;
	*L = *R;
	*R = temp;
	*R ^= P[1];
	*L ^= P[0];

}

/***********************************************************

main.cpp 

Launches the clonefish program.
Processes user input and passes file and key information to 
functions contained within cipher.cpp.
************************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include "util.h"
#include "cipher.h"

int main(int argc, char** argv)
{        
    if (argc > 1) // need at least 2 args to operate
    {
        if (strcmp(argv[1], "-e") == 0 || strcmp(argv[1], "-E") == 0)
        {
        
/////////////////////////////// ENCRYPTION MODE //////////////////////////////////////////
        	
            if (argc < 2) // check for file argument
            {
                cerr << "Please provide a file to be encrypted: " << endl;
                exit(1);
            }
            
            // open input file
            ifstream inFile(argv[2], ios::in | ios::binary | ios::ate);            
            if(!inFile)
            {
                cerr << "File \"" << argv[2] << "\" could not be opened." << endl;
                exit(1);
            }
            
            string outFileName = argv[2];
            outFileName.append(".cfe");
            
            // check for existing ciphertext file
            if (fileExists(outFileName))
            {
            	cout << "A ciphertext for this file currently exists. " << endl;
            	
            	char c = {0};
            	while (true)
            	{
            		cout << "Overwrite? (y/n)    ";
            		
            		string res;
            		getline(cin, res);

					c = res[0];
					if (c == 'Y' || c == 'y')
						break;
					else if (c == 'N' || c == 'n')
						exit(0);
					else
						cout << endl;
				}            		
            }	
            
            // get password, use as key            
            char  pass[MAX_KEY_LEN];
            string passbuf;
            int len = 0;
            while (len < 5 || len > 56)
            {
                passbuf = getpass("Please enter a password (5 -  56 characters): ", true);
                len = passbuf.size();                
            }            
            strcpy(pass, passbuf.c_str());
            
            // open output file
            cout << "Output file: " << outFileName << endl;
            ofstream outFile(outFileName.c_str(), ios::out | ios::binary);
            if(!outFile)
                cerr << "Error creating output file.";
            
            // encrypt file
            cout << "Performing encryption on file \"" << argv[2] << "\"..." << endl; 
            encryptFile(&inFile, &outFile, pass);  
            cout << "Done." << endl;
            inFile.close();
	        outFile.close();
	    }
	    else if (strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "-D") == 0)
	    {
	    
/////////////////////////////// DECRYPTION MODE //////////////////////////////////////////
	        if (argc < 2)
            {
                cerr << "Please provide a file to be decrypted." << endl;
                exit(1);
            }
            
            // open input file
            string in(argv[2]);            
            ifstream inFile(argv[2], ios::in | ios::binary | ios::ate);            
            if(!inFile)
            {
                cerr << "File \"" << argv[2] << "\" could not be opened." << endl;
                exit(1);
            }            
            
            // ensure input file is a .cfe
            string outFileName = argv[2];
            if (outFileName.size() < 5 ||
            	outFileName.substr(outFileName.size() - 4, 4) != ".cfe")
            {
            	cout << "File provided is not in proper format (*.cfe). Aborting...\n";
            	exit(1);
            }
            
            //
            //int pos = outFileName.find(".cfe");
            outFileName = outFileName.substr(0,outFileName.size() - 4);
            
            if (fileExists(outFileName))
            {
            	cout << "A file named \"" << outFileName << "\" already exists. ";
            	
            	char c = {0};
            	while (true)
            	{
            		cout << "Overwrite? (y/n)    ";
            		
            		string res;
            		getline(cin, res);

					c = res[0];
					if (c == 'Y' || c == 'y')
						break;
					else if (c == 'N' || c == 'n')
						exit(0);
					else
						cout << endl;
				}            		
            }
            
            // get password, use as key            
            char pass[MAX_KEY_LEN];
            string passbuf;
            int len = 0;
            while (len < 5 || len > 56)
            {
                passbuf = getpass("Please enter the password for this file: ", true);
                len = passbuf.size();                
            }            
            strcpy(pass, passbuf.c_str());
                                    
            // open output file            
            cout << "Output file: " << outFileName << endl;
                        
            ofstream outFile(outFileName.c_str(), ios::out | ios::binary);
            if(!outFile)
                cerr << "Error creating output file.";    
            
            cout << "Performing decryption on file \"" << argv[2] << "\"..." << endl; 
            decryptFile(&inFile, &outFile, pass);  
            cout << "Done." << endl;
            
            inFile.close();
	        outFile.close();	        
	    }
	    else
	    {
	        cerr << "Option not recognized." << endl;
	        exit(1);
	    }
    }
    else
    {
    	cout << "Please specify a valid option.\nUsage is:\n"
    			"\n\tencrypt: ./clonefish -e (filename)"
    			"\n\tdecrypt: ./clonefish -d (filename) <-- must be a .cfe file\n\n";
    }
    return 0;
}

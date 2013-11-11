/*
    main.cpp
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv)
{
    cout << "Clonefish file encryption program is running.\n" << endl;
    if (argc > 1)
    {                
        cout << "name of file passed as argument: " << argv[1] << endl;//std::string(argv[1]);
        ifstream inFile(argv[1], ios::in | ios::binary);
        
        if(!inFile)
        {
            cerr << "File could not be opened" << endl;
            exit(1);
        }
        
        // open output file
        string outFileName = argv[1];
        outFileName.append(".cfe");
        ofstream outFile(outFileName.c_str(), ios::out | ios::binary);
        if(!outFile)
            cout << "output file could not be created";
        
        // get length of file:
        inFile.seekg (0, inFile.end);
        int length = inFile.tellg();
        inFile.seekg (0, inFile.beg);

        char * buffer = new char [length];

        cout << "Reading " << length << " characters... ";
        // read data as a block:
        inFile.read (buffer,length);

        if (inFile)
          cout << "all characters read successfully.";
        else
          cout << "error: only " << inFile.gcount() << " could be read";
        inFile.close();

        // ...buffer contains the entire file...

        delete[] buffer;        
    }
    else cout << "please provide a file to encrypt. usage is clonefish (filename)" << endl;
    return 0;
}



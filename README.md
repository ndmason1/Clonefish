Clonefish
=========

A file encryption program based on an implementation of Bruce Schneier's Blowfish block cipher.

To build from source, type make in the source directory. The executable is generated in the same directory.

Note that this program uses the openssl library.

The Clonefish file encryption tool runs in one of two modes:

1. File encryption:

	clonefish -e (plaintext filename)
		
After prompting the user for a password of appropriate length,
the specified file is encrypted in 64-bit blocks at a time via 
Blowfish.
		
2. File decryption

	clonefish -d (ciphertext filename)
		
Decrypts the specified .cfe file. Note that the password is not
stored anywhere, so decryption will be performed even if an
incorrect password is entered, resulting in faulty decryption.

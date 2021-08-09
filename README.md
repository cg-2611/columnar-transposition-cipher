# Columnar Transposition Cipher

This was a problem given to me at university.

The program takes a file of text, removes all characters but alphanumeric characters, and encrypts the remaining text using the alphanumeric characters of a given key.

The encryption works by creating a table where the first row is the alphanumeric characters from a given key, and then the alphanumeric characters from the text file are then put into the table beneath the key. In the cases where the table isn't perfectly filled, the empty spaces are padded with 'X' characters. Once the table has been created, the key (first column of the table) is then sorted alphabetically, where whole columns are swapped during the sorting. Finally, the encrypted text is then printed to the terminal.

### Usage
---
The program can be compiled any way you wish, however when running the program, there are some command line arguments that need to the passed in the following order:
- `<text_file_name>`: the name of the text file that contains the the text to be encrypted.
- `<key_for_cipher>`: the key that will be used for the encryption

`example.txt` can be used to demonstrate how the program works. `example.txt` contains a file of random words that are organized into sentences and paragraphs, but do not make sense. Any string can be used as the key, for demonstration purposes, try using "HELLO".

> Note: the key is not required to be uppercase.

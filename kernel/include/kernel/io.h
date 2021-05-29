/*
DAX OS Basic I/O assembly wrapper
Use the following functions to access Input/ Output ports.
Useful for implementing drivers

<rmidhunsuresh@gmail.com>
*/

unsigned char inportb(unsigned int port);

void outportb(unsigned int port, unsigned int value);
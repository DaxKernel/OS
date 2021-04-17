/*
The keyboard driver can optionally accept one callback.
It will call this callback with the character that corresponds to the key that was pressed 
Use this if you need to print the key to the screen
*/

// Uncomment the following macro to disable the optional callback
#define ENABLE_KBD_CALLBACK

#ifdef ENABLE_KBD_CALLBACK
void callback(char code);
#endif
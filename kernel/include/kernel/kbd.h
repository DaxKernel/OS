/*
PS/2 Keyboard Driver for DaxOS
<rmidhunsuresh@gmail.com>
*/

#include <stdbool.h>

void kbd_set_led(bool num, bool caps, bool scroll);
bool kbd_self_test();
void kbd_draw();
void kbd_init();
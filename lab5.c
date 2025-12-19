#include <dos.h>
#include <stdio.h>
#include <conio.h>

void interrupt(*sys_timer)();
unsigned long count = 0;

void interrupt my_timer() {
    count++;
    sys_timer();
}

int main() {
    clrscr();

    sys_timer = getvect(0x08);
    setvect(0x08, my_timer);

    printf("System timer\n");

    while (!kbhit()) {
        gotoxy(1, 3);
        printf("Ticks %lu", count);
    }
    getch();

    setvect(0x08, sys_timer);

    getch();
    return 0;
}
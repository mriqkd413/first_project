#include <stdio.h>
#include <dos.h>
#include <conio.h>

unsigned long timer_ticks = 0;
void interrupt(*old_timer_handler)() = 0;

int seconds = 0;
int minutes = 0;
int need_update = 0;

void interrupt custom_timer_handler()
{
    timer_ticks++;
    if (timer_ticks % 18 == 0) {
        seconds++;
        if (seconds >= 60) {
            seconds = 0;
            minutes++;
        }
        need_update = 1;
    }

    if (old_timer_handler != 0) {
        old_timer_handler();
    }
}

void display_timer()
{
    gotoxy(33, 10);
    textcolor(15);
    textbackground(1);
    cprintf(" %02d:%02d ", minutes, seconds);
    need_update = 0;
}

int main()
{
    old_timer_handler = getvect(0x08);
    setvect(0x08, custom_timer_handler);
    clrscr();

    gotoxy(30, 8);
    textcolor(14);
    textbackground(0);
    cprintf("=== TIMER ===");
    display_timer();

    while (1) {
        if (need_update) {
            display_timer();
        }

        if (kbhit()) {
            if (getch() == 27) break; // ESC
        }

        delay(50);
    }
    setvect(0x08, old_timer_handler);
    textcolor(7);
    textbackground(0);
    clrscr();

    return 0;
}

#include <stdio.h>
#include <conio.h>
#include <dos.h>

#define F1 59   
#define F2 60    
#define ESC 27   

#define X1 20
#define Y1 5
#define X2 60
#define Y2 15

int check_key()
{
    union REGS r;
    r.h.ah = 0x0B;
    int86(0x21, &r, &r);
    return r.h.al;
}

int read_key()
{
    union REGS r;
    r.h.ah = 0x07;
    int86(0x21, &r, &r);
    return r.h.al;
}

int main()
{
    int pos_x = 5, pos_y = 5;
    int key, dir = 0;
    int i = 1;

    int max_x = (X2 - X1) + 1;

    window(X1, Y1, X2, Y2);
    textbackground(WHITE);
    textcolor(MAGENTA);
    _setcursortype(_NOCURSOR);
    clrscr();

    while (i) {
        gotoxy(pos_x, pos_y);
        printf("*");

        if (check_key()) {
            key = read_key();

            if (key == 0 || key == 0xE0) {
                key = read_key();
            }

            switch (key) {
            case F1:
                dir = -1;
                break;

            case F2:
                dir = 1;
                break;

            case ESC:
                i = 0;
                break;

            default:
                dir = 0;
                break;
            }
        }

        if (dir == 1 && pos_x < max_x) {
            pos_x++;
        }
        else if (dir == -1 && pos_x > 1) {
            pos_x--;
        }

        delay(50);
        clrscr();
    }

    window(1, 1, 80, 25);
    textbackground(BLACK);
    textcolor(WHITE);
    _setcursortype(_NORMALCURSOR);
    clrscr();

    return 0;
}
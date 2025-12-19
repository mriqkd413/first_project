#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <string.h>

void scroll(char l_row, char l_col, char r_row, char r_col, char attr)
{
    union REGS r;

    r.h.al = 3;  
    r.h.ah = 6;  

    r.h.ch = l_row;
    r.h.cl = l_col;
    r.h.dh = r_row;
    r.h.dl = r_col;
    r.h.bh = attr;
    int86(0x10, &r, &r);
}

void printText(int bg, int fg) {
    char text[50];
    sprintf(text, "Background=%d Text=%d", bg, fg);
    cprintf("%s", text);
}

void setCursorPos(int row, int col) {
    union REGS r;
    r.h.ah = 2;
    r.h.bh = 0;
    r.h.dh = row;
    r.h.dl = col;
    int86(0x10, &r, &r);
}

int main() {
    int bg, fg, line;
    int x1 = 20, y1 = 5, x2 = 60, y2 = 15;

    union REGS r;
    r.h.ah = 0;
    r.h.al = 3;  
    int86(0x10, &r, &r);
    
    for (bg = 0; bg < 8; bg++) {
        for (fg = 0; fg < 16; fg += 3) {
            scroll(y1, x1, y2, x2, bg << 4);

            for (line = 0; line < 3; line++) {
                int current_fg = fg + line;
                if (current_fg >= 16) break;

                setCursorPos(y2 - 2 + line, x1);
                textattr((bg << 4) | current_fg);
                printText(bg, current_fg);
            }

            delay(500);
        }
    }
    getch();
    return 0;
}
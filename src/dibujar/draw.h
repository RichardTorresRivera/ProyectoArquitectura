#ifndef DRAW_H
#define DRAW_H

void drawClock(bool fullRedraw);
void drawMenu(bool fullRedraw, int items[]);
// items
void drawChronometer(bool fullRedraw, unsigned long elapsedTime);

#endif

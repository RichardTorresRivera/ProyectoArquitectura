#ifndef DRAW_H
#define DRAW_H

// Pantallas
void drawClock(bool fullRedraw);
void drawMenu(bool fullRedraw, int items[]);
void drawCallMissed(const char *contact, const char *number);
// Items
void drawChronometer(bool fullRedraw, unsigned long elapsedTime);
void drawMusic(bool fullRedeaw);
void drawAlarm(bool fullRedraw);
void drawGame(bool fullRedraw);
void drawNotifications(bool fullRedraw);

#endif

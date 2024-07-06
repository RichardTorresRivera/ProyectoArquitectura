#ifndef DRAW_H
#define DRAW_H

// Pantallas
void drawClock(bool fullRedraw);
void drawMenu(bool fullRedraw, int items[]);
void drawCallMissed(const char *contact, const char *number);
void drawBattery(const char *n);
void drawLoad(bool fullRedraw, unsigned int &progress);
// Items
void drawChronometer(bool fullRedraw, unsigned long elapsedTime);
void drawMusic(bool fullRedeaw);
void drawAlarm(bool fullRedraw);
void drawGame(bool fullRedraw);
void drawNotifications(const char *msg);
void drawSoundMovil(bool fullRedraw);

#endif

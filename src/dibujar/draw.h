#ifndef DRAW_H
#define DRAW_H

// Pantallas
void drawClock(bool fullRedraw);
void drawMenu(bool fullRedraw, int items[]);
void drawCallMissed(const char *contact, const char *number);
void drawBatteryLow(const char *n);
void drawLoad(bool fullRedraw, unsigned int &progress);
void drawMenuSound(bool fullRedraw, int items[]);
// Items
void drawChronometer(bool fullRedraw, unsigned long elapsedTime);
void drawMusic(bool fullRedeaw);
void drawAlarm(bool fullRedraw, const String &hour, const String &minute, const String &day);
void drawTask(bool fullRedraw, const String &dueDate, const std::vector<String> &tasks);
void drawNotifications(const char *app, const char *msg);
void drawSoundMovil(bool fullRedraw, short n);

#endif

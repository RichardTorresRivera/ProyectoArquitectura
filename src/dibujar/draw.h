#ifndef DRAW_H
#define DRAW_H

// Pantallas
void drawClock(bool fullRedraw);
void drawMenu(bool fullRedraw, int items[]);
void drawCallMissed(const String contact, const String number);
void drawBatteryLow(const String n);
void drawLoad(bool fullRedraw, unsigned short &progress);
void drawMenuSound(bool fullRedraw, int items[]);
// Items
void drawChronometer(bool fullRedraw, unsigned long elapsedTime);
void drawMusic(bool fullRedeaw);
void drawAlarm(bool fullRedraw, const String hour, const String minute);
void drawTask(bool fullRedraw, const String dueDate, const std::vector<String> tasks);
void drawNotifications(const String app, const String msg);
void drawSoundMovil(bool fullRedraw, short n);

#endif

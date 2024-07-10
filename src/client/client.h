#ifndef CLIENT_H
#define CLIENT_H

#include <HTTPClient.h>
#include "declarations.h"

void playPauseMusic(bool &play_pause_music);
void nextMusic();
void toneMovil(short n);
AlarmTime getAlarm();
bool isAlarmTime(NTPClient ntpClient, AlarmTime alarmTime, bool &alarmActivated);
Task getTask();

#endif // CLIENT_H
#ifndef __MAMEIO_H
#define __MAMEIO_H

#include <cstdio>

#include <Path.h>

#define MAX_ROMNAME 10
#define MAX_GAMENAME 100
#define MAX_MANU 50
#define MAX_YEAR 5
#define MAX_STATUS 20
#define MAX_CPU 20
#define MAX_CONTROL 20
#define MAX_CATEGORY 40
#define MAX_NUMPLAYERS 3
#define MAX_NUMBUTTONS 3
#define MAX_MAMEVER 20
#define MAX_FAVORITE 10
#define MAX_DRIVERSTATUS 20
#define MAX_TIMESPLAYED 20

struct Rom {
	Rom();
	void Clear();
	char romName[MAX_ROMNAME];
	char description[MAX_GAMENAME];
	char cloneOf[MAX_ROMNAME];
	char year[MAX_YEAR];
	char manufacturer[MAX_MANU];
	char cpu1[MAX_CPU];
	char cpu2[MAX_CPU];
	char cpu3[MAX_CPU];
	char cpu4[MAX_CPU];
	char sound1[MAX_CPU];
	char sound2[MAX_CPU];
	char sound3[MAX_CPU];
	char sound4[MAX_CPU];
	char control[MAX_CONTROL];
	char numButtons[MAX_NUMBUTTONS];
	char numPlayers[MAX_NUMPLAYERS];
};

class BColumnListView;
class BTextView;
class TMameInterface {
public:
	TMameInterface(const char *executablePath = NULL, const char *destPath = NULL);
	~TMameInterface();
	status_t InitCheck();
	void Rewind();
	status_t GetNextRom(Rom *rom);
	status_t GetRomList(const char *settingsPath, bool generate, BColumnListView *romList);
	status_t CheckSet(const char *setName);
	status_t MoreCheckSet(const char *setName, BTextView *output);
	status_t Play(const char *options, BString *output);
	
private:
	static BPath sMamePath;
	static BPath sDestPath;
	static bool sGenerated;
	FILE *fRomFile;
};

#endif

#include "Settings.h"

#include <Directory.h>
#include <Entry.h>
#include <File.h>
#include <FindDirectory.h>
#include <Message.h>

#include <cstdio>

const char *kDirName = "BeMAMEr";
const char *kFileName = "settings";
const int32 kSettingsMessage = 'StMs';

BRect Settings::sWindowFrame;

BPath Settings::sPath;
BPath Settings::sMamePath;
BPath Settings::sShotsPath;
bool Settings::sStretchShots;

int32 Settings::sLastSelectedGame;
BList Settings::sFavorites;
int32 Settings::sViewFilter;
bool Settings::sIsExpanded;
BMessage Settings::sListViewState;

BMessage Settings::sSettings(kSettingsMessage);
bool Settings::sLoaded = false;


Settings::Settings()
{
	if (!sLoaded)
		Load();
}


status_t
Settings::Load()
{
	sLoaded = true;	
	
	status_t result = find_directory(B_USER_SETTINGS_DIRECTORY, &sPath);
	if (result == B_OK)
		result = sPath.Append(kDirName);
	
	if (!BEntry(sPath.Path()).Exists() && result == B_OK) {
		result = create_directory(sPath.Path(), 777);
		return B_ERROR;
	}
	
	BPath filePath(sPath.Path());
	if (result == B_OK)
		result = filePath.Append(kFileName);
	
	if (!BEntry(filePath.Path()).Exists())
		return B_ERROR;
	
	BFile settingsFile;
	if (result == B_OK)
		settingsFile.SetTo(filePath.Path(), B_READ_ONLY);
		
	if (settingsFile.InitCheck() == B_OK)
		result = sSettings.Unflatten(&settingsFile);
		
	if (result == B_OK) {
		sSettings.FindRect("frame", &sWindowFrame);
		sSettings.FindInt32("lastSelected", &sLastSelectedGame);
		sSettings.FindInt32("viewFilter", &sViewFilter);		
		sSettings.FindBool("stretchShots", &sStretchShots);
		sSettings.FindBool("expanded", &sIsExpanded);
		sSettings.FindMessage("listViewState", &sListViewState);
		
		const char *path = NULL;
		if (sSettings.FindString("mamePath", &path) == B_OK)
			sMamePath.SetTo(path);
		if (sSettings.FindString("shotsPath", &path) == B_OK)
			sShotsPath.SetTo(path);
		
	}			
	return result;
}


status_t
Settings::Save()
{
	BEntry entry(sPath.Path());
	BPath filePath(sPath.Path());
	filePath.Append(kFileName);
	
	BFile settingsFile(filePath.Path(), B_WRITE_ONLY|B_CREATE_FILE);
	
	if (settingsFile.InitCheck() == B_OK) {
		sSettings.MakeEmpty();
		sSettings.AddRect("frame", sWindowFrame);
		sSettings.AddString("mamePath", sMamePath.Path());
		sSettings.AddString("shotsPath", sShotsPath.Path());
		sSettings.AddInt32("lastSelected", sLastSelectedGame);
		
		sSettings.AddBool("stretchShots", sStretchShots);
		sSettings.AddBool("expanded", sIsExpanded);
		sSettings.AddMessage("listViewState", &sListViewState);
		
		//sSettings.PrintToStream();
		
		sSettings.Flatten(&settingsFile);
		return B_OK;
	}	
	
	return B_ERROR;
}


BRect
Settings::WindowFrame() const
{
	return sWindowFrame;
}


void
Settings::SetWindowFrame(BRect frame)
{
	sWindowFrame = frame;
}


BPath
Settings::Path() const
{
	return sPath;
}


BPath
Settings::MamePath() const
{
	return sMamePath;
}


void
Settings::SetMamePath(BPath &path)
{
	sMamePath.SetTo(path.Path());
}


BPath
Settings::ShotsPath() const
{
	return sShotsPath;
}


void
Settings::SetShotsPath(BPath &path)
{
	sShotsPath.SetTo(path.Path());
	printf("Shots Path: %s\n", sShotsPath.Path());
}


bool
Settings::StretchShots() const
{
	return sStretchShots;
}


void
Settings::SetStretchShots(bool set)
{
	sStretchShots = set;
}


int32
Settings::LastSelectedGame() const
{
	return sLastSelectedGame;
}


void
Settings::SetLastSelectedGame(int32 index)
{
	sLastSelectedGame = index;
}


void
Settings::SetViewFilter(int32 index)
{
	sViewFilter = index;
}


int32
Settings::ViewFilter() const
{
	return sViewFilter;
}


bool
Settings::IsExpanded() const
{
	return sIsExpanded;
}


void
Settings::SetExpanded(bool state)
{
	sIsExpanded = state;
}


BMessage *
Settings::ListViewState() const
{
	return &sListViewState;
}


void
Settings::SetListViewState(BMessage *message)
{
	sListViewState = *message;
}

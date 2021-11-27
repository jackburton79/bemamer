#ifndef __SETTINGS_H
#define __SETTINGS_H

#include <List.h>
#include <Message.h>
#include <Path.h>

class Settings
{
public:
	Settings();
	static status_t Load();
	static status_t Save();
	
	BRect WindowFrame() const;
	void SetWindowFrame(BRect rect);
	
	BPath Path() const;
	
	BPath MamePath() const;
	void SetMamePath(BPath &);
	
	BPath ShotsPath() const;
	void SetShotsPath(BPath &);
	
	bool StretchShots() const;
	void SetStretchShots(bool );
	
	int32 LastSelectedGame() const;
	void SetLastSelectedGame(int32 i);
	
	status_t GetFavorites(BList *) const;
	void SetFavorites(BList *);
	
	bool IsExpanded() const;
	void SetExpanded(bool);
	
	BMessage *ListViewState() const;
	void SetListViewState(BMessage *message);
	
	int32 ViewFilter() const;
	void SetViewFilter(int32);
			
private:
	static BRect sWindowFrame;
	
	static BPath sPath;
	static BPath sMamePath;
	static BPath sShotsPath;
	static bool sStretchShots;
	
	static int32 sLastSelectedGame;
	static BList sFavorites;
	static int32 sViewFilter;
	static bool sIsExpanded;
	static BMessage sListViewState;
	
	static BMessage sSettings;
	static bool sLoaded;
};

#endif // __SETTINGS_H

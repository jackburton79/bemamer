#ifndef __BEMAMERWINDOW_H
#define __BEMAMERWINDOW_H

#include <List.h>
#include <Window.h>

class BBox;
class BButton;
class BColumnListView;
class BStatusBar;
class BTabView;
class GameItem;
class BeMamerWindow : public BWindow
{
public:
	BeMamerWindow();
	virtual void Show();
	virtual bool QuitRequested();
	virtual void MessageReceived(BMessage *message);
	virtual void MenusBeginning();
	
private:
	BBox *fBackgroundView;
	BBox *fListBox;
	BColumnListView *fListView;
	BStatusBar *fStatus;
	BButton *fPlayButton;
	BTabView *fTabs;
	
	BList fAvailable;
	BList fNotAvailable;
	
	bool fLVIsExpanded;
	bool fFirstShow;
		
	thread_id fRomGenerator;
	thread_id fRomChecker;
	
	bool fKillThread;
	
	float AddMenuBar();	
	static int32 CreateGameList(void *arg);
	static int32 CheckRoms(void *arg);
	status_t Play();
	void SelectRandomGame();
};

#endif // __BEMAMERWINDOW_H

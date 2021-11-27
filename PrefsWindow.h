#ifndef __PREFSWINDOW_H
#define __PREFSWINDOW_H

#include <Window.h>

class BTabView;
class TPrefsWindow : public BWindow
{
public:
	TPrefsWindow();
	virtual bool QuitRequested();
	virtual void MessageReceived(BMessage *);
private:
	BTabView *fTabView;
};

#endif

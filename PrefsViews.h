#ifndef __PREFSVIEWS_H
#define __PREFSVIEWS_H

#include <View.h>

class BButton;
class BTextControl;
class TFileLocationsView : public BView
{
public:
	TFileLocationsView(BRect rect);
	virtual void DetachedFromWindow();
	virtual void MessageReceived(BMessage *message);
private:
	BTextControl *fTCDefaultExe;
	BButton *fBrowseExe;
	BTextControl *fTCRomsFolder;
	BTextControl *fTCSnapsFolder;
	BButton *fBrowseSnaps;
	
	typedef BView _inherited;
};

class BCheckBox;
class TGeneralOptionsView : public BView
{
public:
	TGeneralOptionsView(BRect rect);
	virtual void DetachedFromWindow();
private:
	BCheckBox *fStretch;
	
	typedef BView _inherited;
};

#endif

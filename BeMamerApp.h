#ifndef __BEMAMERAPP_H
#define __BEMAMERAPP_H

#include <Application.h>
#include <FilePanel.h>
#include <Path.h>

extern const char *kAppSignature;
extern BFilePanel *gFilePanel;

class BWindow;
class BeMamerApp : public BApplication
{
public:
	BeMamerApp();
	virtual void ReadyToRun();
	virtual void MessageReceived(BMessage *message);
	virtual void Quit();
	virtual void AboutRequested();

	BPath Path() const;
	
private:
	BWindow *fMainWindow;
	BWindow *fPrefsWindow;
	BPath fPath;
	bool fFirstRun;
	
	void SetDefaults();
};


#endif // __BEMAMERAPP_H

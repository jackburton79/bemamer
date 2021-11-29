#include "BeMamerApp.h"
#include "BeMamerWindow.h"
#include "Messages.h"
#include "PrefsWindow.h"
#include "Settings.h"
//#include "RectUtils.h"

#include <Alert.h>
#include <FilePanel.h>
#include <Roster.h>

const char *kAppSignature = "application/x-vnd.BeMameR";
BFilePanel *gFilePanel;

int
main()
{
	BeMamerApp app;
	app.Run();
	
	return 0;
}


BeMamerApp::BeMamerApp()
	:BApplication(kAppSignature),
	fPrefsWindow(NULL)
{
	app_info info;
	if (be_app->GetAppInfo(&info) == B_OK) {
		entry_ref ref = info.ref;
		BEntry entry;
		if (entry.SetTo(&ref) == B_OK) {
			if (fPath.SetTo(&entry) == B_OK) {
				if (fPath.GetParent(&fPath) == B_OK) {
					//chdir(fPath.Path());
				}
			}
		}
	}
	
	gFilePanel = new BFilePanel();
	
	if (Settings::Load() != B_OK) {
		SetDefaults();
		(new BAlert("Error",
			"BeMameR can't find its settings file, default values will be used. You can change them in the options window that will be shown just after this alert.",
			"Ok", NULL, NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
		fPrefsWindow = new TPrefsWindow();
		fPrefsWindow->Show();
		fFirstRun = true;
	};
	
	if (!fFirstRun)
		fMainWindow = new BeMamerWindow;
}


void
BeMamerApp::ReadyToRun()
{
	if (!fFirstRun)
		fMainWindow->Show();

	BApplication::ReadyToRun();
}


void
BeMamerApp::MessageReceived(BMessage *message)
{
	switch(message->what) {
		case kOpenPrefsWindow:
			fPrefsWindow = new TPrefsWindow();
			fPrefsWindow->Show();
			break;
		case kPrefsWindowHasQuit:
			if (fFirstRun) {
				fMainWindow = new BeMamerWindow();
				fMainWindow->Show();
				fFirstRun = false;
			} else
				fMainWindow->PostMessage(kTabChanged);

		default:
			BApplication::MessageReceived(message);
			break;
	}
}


void
BeMamerApp::Quit()
{
	Settings::Save();
	BApplication::Quit();
}


void
BeMamerApp::AboutRequested()
{
	(new BAlert("Info", "BeMameR\n\nby Stefano Ceccherini", "Ok"))->Go(NULL);
}


BPath
BeMamerApp::Path() const
{
	return fPath;
}


void
BeMamerApp::SetDefaults()
{
	Settings settings;
	BRect frame(0, 0, 600, 400);
	
	settings.SetWindowFrame(frame);
	BPath tmpPath(fPath);
	tmpPath.Append("mame");
	settings.SetMamePath(tmpPath);
	tmpPath.SetTo(fPath.Path());
	tmpPath.Append("SNAP");
	settings.SetShotsPath(tmpPath);
	settings.SetStretchShots(false);
	settings.SetExpanded(false);
}

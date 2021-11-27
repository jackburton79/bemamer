#include "BeMamerApp.h"
#include "Messages.h"
#include "PrefsViews.h"
#include "PrefsWindow.h"
#include "Settings.h"
#include "RectUtils.h"

#include <Box.h>
#include <Button.h>
#include <TabView.h>

#include <cstdio>

const BRect kFrame(0, 0, 400, 250);

TPrefsWindow::TPrefsWindow()
	:BWindow(kFrame, "Options Window", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	center_window_on_screen(this);	
	
	BBox *box = new BBox(Bounds(), "Background");
	box->ResizeBy(1, 1);
	
	fTabView = new BTabView(Bounds().InsetByCopy(10, 10), "Tabs view");
	
	box->AddChild(fTabView);
	
	BRect contentRect(fTabView->ContainerView()->Bounds());	
	fTabView->AddTab(new TGeneralOptionsView(contentRect));
	fTabView->AddTab(new TFileLocationsView(contentRect));	
	AddChild(box);
}


bool
TPrefsWindow::QuitRequested()
{
	be_app->PostMessage(kPrefsWindowHasQuit);
	return BWindow::QuitRequested();
}


void
TPrefsWindow::MessageReceived(BMessage *message)
{
	switch(message->what) {
		case kBrowse:
		{
			BButton *source;
			if (message->FindPointer("source", (void **)&source) != B_OK)
				break;
			if (strcmp(source->Name(), "BrowseSnaps") == 0) {
				BMessage message(kFPSnapsSelected);
				gFilePanel->SetMessage(&message); 
				gFilePanel->SetTarget(source->Parent());			
			}
			gFilePanel->Show();			
			break;
		}
		default:
			BWindow::MessageReceived(message);
			break;
	}
}


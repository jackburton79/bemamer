#include "BeMamerApp.h"
#include "Messages.h"
#include "PrefsViews.h"
#include "Settings.h"

#include <Box.h>
#include <Button.h>
#include <CheckBox.h>
#include <TextControl.h>

#include <cstdio>

TFileLocationsView::TFileLocationsView(BRect rect)
	:BView(rect, "File Locations", B_FOLLOW_NONE, B_WILL_DRAW)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	BRect TCRect(6, 5, Bounds().right - 60, 30);
	fTCDefaultExe = new BTextControl(TCRect,
		"Default Executable", "Default Executable:", "", NULL );
	float divider = StringWidth(fTCDefaultExe->Label()) + 2;
	fTCDefaultExe->SetDivider(divider);
	fTCDefaultExe->SetText(Settings().MamePath().Path());
	
	AddChild(fTCDefaultExe);
	
	BRect buttonRect = fTCDefaultExe->Frame();
	buttonRect.left = buttonRect.right + 5;
	buttonRect.right = Bounds().right - 5;
	buttonRect.OffsetBy(0, -2);
	
	/*fBrowseExe = new BButton(buttonRect, "BrowseExe", "Browse", new BMessage(kBrowse));
	fBrowseExe->SetTarget(this);
	AddChild(fBrowseExe);
	*/
	TCRect.OffsetBy(0, 40);
	fTCRomsFolder = new BTextControl(TCRect, 
		"Roms folders", "Roms folders:", "", NULL );
	fTCRomsFolder->SetDivider(divider);
	fTCRomsFolder->SetEnabled(false);
	AddChild(fTCRomsFolder);
	
	TCRect.OffsetBy(0, 20);
	fTCSnapsFolder = new BTextControl(TCRect, 
		"Snapshots folder", "Snapshots folder:", "", NULL );
	fTCSnapsFolder->SetDivider(divider);
	fTCSnapsFolder->SetText(Settings().ShotsPath().Path());
	AddChild(fTCSnapsFolder);
	
	buttonRect.OffsetBy(0, 60);
	//fBrowseSnaps = new BButton(buttonRect, "BrowseSnaps", "Browse", new BMessage(kBrowse));
	//fBrowseSnaps->SetTarget(this);
	//AddChild(fBrowseSnaps);
}


void
TFileLocationsView::DetachedFromWindow()
{
	BPath path(fTCDefaultExe->Text());
	Settings().SetMamePath(path);
	path.SetTo(fTCSnapsFolder->Text());
	Settings().SetShotsPath(path);
	_inherited::DetachedFromWindow();
}


void
TFileLocationsView::MessageReceived(BMessage *message)
{
	switch(message->what) {
		case kFPSnapsSelected:
		{
			printf("FPSnapsSelected");
			entry_ref ref;
			if (message->FindRef("refs", &ref) == B_OK) {
				fTCSnapsFolder->SetText(ref.name);			
			}
			break;
		}
		default:
			_inherited::MessageReceived(message);
			break;
	}
}


TGeneralOptionsView::TGeneralOptionsView(BRect rect)
	:BView(rect, "General Options", B_FOLLOW_NONE, B_WILL_DRAW)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	BRect CBRect(6, 5, Bounds().right - 10, 30);
	fStretch = new BCheckBox(CBRect, "Stretch", "Auto-size screenshots", 
		new BMessage());
	
	fStretch->SetValue(Settings().StretchShots() ? B_CONTROL_ON : B_CONTROL_OFF);
	AddChild(fStretch);
}


void
TGeneralOptionsView::DetachedFromWindow()
{
	Settings().SetStretchShots((fStretch->Value() == B_CONTROL_ON) ? true : false);
	_inherited::DetachedFromWindow();
}

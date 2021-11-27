#include "Messages.h"
#include "VerifyView.h"

#include <Button.h>
#include <Rect.h>
#include <ScrollView.h>
#include <String.h>
#include <Window.h>

#include <cstdio>

VerifyView::VerifyView(BRect rect)
			:BView(rect, "Verify", B_FOLLOW_ALL_SIDES, B_WILL_DRAW)
{
	BRect bounds(Bounds());
	bounds.InsetBy(13, 13);
	
	BRect reportRect(bounds);
	BRect buttonRect(bounds);
	reportRect.top += 50;
	reportRect.right -= 10;
	buttonRect.bottom = reportRect.top - 20;
	buttonRect.top = buttonRect.bottom - 30;
	
 	fReport = new BTextView(reportRect, "Report", Bounds(), B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	fReport->SetFontAndColor(be_fixed_font);
	fReport->MakeEditable(false);
	fButton = new BButton(buttonRect, "verifybut", NULL,
				new BMessage(kVerifyRomset), B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW);
	BScrollView *scrollView = new BScrollView("Scroll View", fReport,
		B_FOLLOW_ALL_SIDES, B_WILL_DRAW, false, true);
	
	fButton->SetEnabled(false);
	
	AddChild(scrollView);
	AddChild(fButton);
}


void
VerifyView::AttachedToWindow()
{
	BView::AttachedToWindow();
	
	SetViewColor(Parent()->ViewColor());
	BRect parentRect(Parent()->Bounds());
	ResizeTo(parentRect.Width(), parentRect.Height());
	fReport->SetViewColor(255, 255, 255);	
}


void
VerifyView::MessageReceived(BMessage *message)
{
	switch(message->what) {
		case kLVSelectionChanged:
		case kTabChanged:
		{
			const char *gameName = NULL;
			if (message->FindString("gameName", &gameName) == B_OK) {
				BString label("Verify and list roms for ");
				label.Append(gameName);
				fButton->SetLabel(label.String());
				fButton->SetEnabled(true);
			}
			break;
		}
		default:
			BView::MessageReceived(message);
			break;
	}
}

#include "GameAlert.h"

#include <cstdio>

#include <Box.h>
#include <Button.h>
#include <ScrollView.h>
#include <String.h>
#include <StringView.h>
#include <TextView.h>

TGameAlert::TGameAlert(BString string)
	:BWindow(BRect(0, 0, 350, 300), "Game Alert",
		B_MODAL_WINDOW, B_ASYNCHRONOUS_CONTROLS|B_NOT_RESIZABLE)
{
	//center_window_on_screen(this);
	BRect bounds(Bounds());
	bounds.InsetBy(-1, -1);
	BBox *background = new BBox(bounds, "background");
	
	BRect labelRect(10, 5, bounds.right - 10, 15);
	BStringView *label = new BStringView(labelRect, "label", "");
	label->SetText("The selected game cannot be run, the following files are missing:");
	
	BRect tvRect(bounds);
	tvRect.top = labelRect.bottom + 5;
	tvRect.bottom = bounds.bottom - 40;
	tvRect.right -= B_V_SCROLL_BAR_WIDTH + 10;
	tvRect.left += 10;
			
	fTextView = new BTextView(tvRect, "text view", Bounds(), B_FOLLOW_NONE, B_WILL_DRAW);
	fTextView->MakeEditable(false);
	fTextView->SetFontAndColor(be_fixed_font);
	fTextView->SetText(string.String());
	BScrollView *scrollView = new BScrollView("", fTextView,
		B_FOLLOW_LEFT | B_FOLLOW_TOP, 0, false, true);
	
	BRect buttonRect(fTextView->Frame());
	buttonRect.top = buttonRect.bottom + 25;
	buttonRect.right = bounds.right - 7;
	buttonRect.left = buttonRect.right - 70;
	buttonRect.bottom = bounds.bottom - 5;
	
	BButton *exitButton = new BButton(buttonRect, "exit",
		"Ok", new BMessage(B_QUIT_REQUESTED));
	
	background->AddChild(label);
	background->AddChild(exitButton);
	background->AddChild(scrollView);
	
	AddChild(background);
}


BTextView *
TGameAlert::TextView()
{
	return fTextView;
}

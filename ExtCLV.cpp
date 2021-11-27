#include "ColumnTypes.h"
#include "ExtCLV.h"

#include <cstdio>

#include <MessageRunner.h>
#include <PopUpMenu.h>

TExtColumnListView::TExtColumnListView(BRect rect,
					const char *name,
					uint32 resizingMode,
					uint32 drawFlags,
					border_style style,
					bool showHorizontalScrollbar)
				
	:
	BColumnListView(rect, name, resizingMode, drawFlags, style, showHorizontalScrollbar)
{
	fKeyRunner = NULL;
	//fMatchingColumn = 0;
}


void
TExtColumnListView::KeyDown(const char *bytes, int32 numBytes)
{	
	// Code taken from opentracker			
	char key = bytes[0];

	switch (key) {
		case B_LEFT_ARROW:
		case B_RIGHT_ARROW:
		case B_UP_ARROW:
		case B_DOWN_ARROW:
		case B_RETURN:
		case B_HOME:
		case B_END:
		case B_PAGE_UP:
		case B_PAGE_DOWN:
		case B_TAB:
			BColumnListView::KeyDown(bytes, numBytes);
			break;
		case B_BACKSPACE:
			// remove last char from the typeahead buffer
			fBuffer.Truncate(fBuffer.Length() - 1);
			sLastKeyTime = system_time();				
			// select our new string
			SelectBestMatching();	
			break;
		default: {	
			bigtime_t doubleClickSpeed;
			get_click_speed(&doubleClickSpeed);

			// start watching
			if (fKeyRunner == NULL) {
				fKeyRunner = new BMessageRunner(this, new BMessage(kCheckTypeahead), doubleClickSpeed);
				if (fKeyRunner->InitCheck() != B_OK)
					return;
			}
						
			fBuffer.Append(bytes, numBytes);
			sLastKeyTime = system_time();
			
			SelectBestMatching();
		}
	}
}


void
TExtColumnListView::MouseDown(BPoint where)
{
	BColumnListView::MouseDown(where);
	BPoint loc;
	uint32 buttons;
	
	//GetMouse(&loc, &buttons);
	//if (buttons & B_SECONDARY_MOUSE_BUTTON)
	//	fPopupMenu->Go(ConvertToScreen(BPoint(4, 4)));
}


void
TExtColumnListView::MessageReceived(BMessage *message)
{
	switch (message->what) {
		// Code taken from opentracker
		case kCheckTypeahead: {
			bigtime_t doubleClickSpeed;
			get_click_speed(&doubleClickSpeed);
			if (system_time() - sLastKeyTime > (doubleClickSpeed * 2)) {
				fBuffer.SetTo("");
				delete fKeyRunner;
				fKeyRunner = NULL;
			}
			break;
		}
		default:
			BColumnListView::MessageReceived(message);
			break;
	}
}


void
TExtColumnListView::SelectBestMatching(void)
{
	int32 len = fBuffer.Length();
	
	for (int32 index = 0; index < CountRows(); index++) {
		BRow *currentRow = RowAt(index);
		BStringField *field = dynamic_cast<BStringField *>(currentRow->GetField(0));
		if (field != NULL && fBuffer.ICompare(field->String(), len) == 0) {
			if (currentRow != CurrentSelection()) {
				SetFocusRow(currentRow, true);
				ScrollTo(currentRow);
				break;
			}
		}
	}
}


bigtime_t TExtColumnListView::sLastKeyTime = 0;

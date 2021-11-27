#include "InfoView.h"
#include "Messages.h"

#include <Box.h>
#include <String.h>
#include <TextControl.h>

#include <cstdio>

InfoView::InfoView(BRect size)
		:BView(size, "Game Info", B_FOLLOW_ALL_SIDES, B_WILL_DRAW)
{
	BRect infoRect(10, 10, 180, 245);
	fInfoBox = new BBox(infoRect, "infobox",
				B_FOLLOW_LEFT|B_FOLLOW_TOP, B_WILL_DRAW);
	fInfoBox->SetLabel("Driver Info");
	
	BRect rect(infoRect.top + 2, infoRect.left,
				infoRect.right - 16, infoRect.bottom);
	rect.bottom = rect.top + 20;

	fTextControl[0] = new BTextControl(rect, "cpu1", "Cpu 1:", NULL, NULL);
	fTextControl[1] = new BTextControl(rect, "cpu2", "Cpu 2:", NULL, NULL);
	fTextControl[2] = new BTextControl(rect, "cpu3", "Cpu 3:", NULL, NULL);
	fTextControl[3] = new BTextControl(rect, "cpu4", "Cpu 4:", NULL, NULL);
	fTextControl[4] = new BTextControl(rect, "sound1", "Sound 1:", NULL, NULL);
	fTextControl[5] = new BTextControl(rect, "sound2", "Sound 2:", NULL, NULL);
	fTextControl[6] = new BTextControl(rect, "sound3", "Sound 3:", NULL, NULL);
	fTextControl[7] = new BTextControl(rect, "sound4", "Sound 4:", NULL, NULL);
	fTextControl[8] = new BTextControl(rect, "control", "Control:", NULL, NULL);
	fTextControl[9] = new BTextControl(rect, "numbutt", "Buttons:", NULL, NULL);
	fTextControl[10] = new BTextControl(rect, "numplay", "Players:", NULL, NULL);
	
	float offset = 0;
	
	for (int32 c = 0; c < 11; c++) {
		fInfoBox->AddChild(fTextControl[c]);
		fTextControl[c]->MoveBy(0, offset);
		fTextControl[c]->SetEnabled(false);
		offset += 20;
	}
	AddChild(fInfoBox);
}


void
InfoView::AttachedToWindow()
{	
	if (Parent())
		SetViewColor(Parent()->ViewColor());	
	BView::AttachedToWindow();
}


void
InfoView::MessageReceived(BMessage *message)
{
	switch(message->what) {
		case kLVSelectionChanged:
		case kTabChanged:
		{	
			const char *string = NULL;
			int32 tmp;
			char tmpString[64];
			if (message->FindString("cpu1", &string) == B_OK)
				fTextControl[0]->SetText(string);
			if (message->FindString("cpu2", &string) == B_OK)
				fTextControl[1]->SetText(string);
			if (message->FindString("cpu3", &string) == B_OK)
				fTextControl[2]->SetText(string);
			if (message->FindString("cpu4", &string) == B_OK)
				fTextControl[3]->SetText(string);
			if (message->FindString("sound1", &string) == B_OK)
				fTextControl[4]->SetText(string);
			if (message->FindString("sound2", &string) == B_OK)
				fTextControl[5]->SetText(string);
			if (message->FindString("sound3", &string) == B_OK)
				fTextControl[6]->SetText(string);
			if (message->FindString("sound4", &string) == B_OK)
				fTextControl[7]->SetText(string);
			if (message->FindString("control", &string) == B_OK)
				fTextControl[8]->SetText(string);
			if (message->FindInt32("numButtons", &tmp) == B_OK) {
				snprintf(tmpString, 64, "%ld", tmp);
				fTextControl[9]->SetText(tmpString);
			}
			if (message->FindInt32("numPlayers", &tmp) == B_OK) {
				snprintf(tmpString, 64, "%ld", tmp);
				fTextControl[10]->SetText(tmpString);
			}
			break;
		}
		default:
			BView::MessageReceived(message);
			break;
	}
}

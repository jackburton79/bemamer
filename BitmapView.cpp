#include "BitmapView.h"
#include "Messages.h"
#include "Settings.h"
#include "RectUtils.h"

#include <Bitmap.h>
#include <String.h>
#include <TextControl.h>
#include <TranslationUtils.h>
#include <Window.h>

#include <cstdio>

BitmapView::BitmapView(BRect rect)
			:BView(rect, "Screen Shot", B_FOLLOW_ALL_SIDES, 
				B_WILL_DRAW|B_FRAME_EVENTS)
{
	BRect bounds(Bounds().InsetByCopy(5, 5));
	fBitmapView = new BView(bounds, "Bitmap View", B_FOLLOW_ALL_SIDES,
		B_WILL_DRAW|B_FRAME_EVENTS);
	AddChild(fBitmapView);	
}


void
BitmapView::AttachedToWindow()
{
	BView *parent = Parent();
	if (parent != NULL) {
		SetViewColor(parent->ViewColor());
		fBitmapView->SetViewColor(parent->ViewColor());
		BRect parentRect(parent->Bounds());
		ResizeTo(parentRect.Width(), parentRect.Height());
	}
	BView::AttachedToWindow();
	
	
}


const char *
kTypes[] = { ".png", ".bmp", ".pcx", ".gif", NULL };


void
BitmapView::MessageReceived(BMessage *message)
{
	switch(message->what) {
		case kLVSelectionChanged:
		case kTabChanged:
		{
			fBitmapView->ClearViewBitmap();
			
			const char *names[3];
			
			message->FindString("romName", &names[0]);
			message->FindString("cloneOf", &names[1]); 
			names[2] = NULL;
			
			BPath pathObj(Settings().ShotsPath());
			const char *path = pathObj.Path();
			BBitmap *bitmap = NULL;
			BString filename;
			BString filenameExt;
	
			for (int32 c = 0; names[c] != NULL; c++) {
				filename.SetTo(path);
				filename << "/" << names[c];
				for (int32 count = 0; kTypes[count] != NULL; count++) {
					filenameExt.SetTo(filename.String());
					filenameExt << kTypes[count];
					printf("name: %s\n", filenameExt.String());
					bitmap = BTranslationUtils::GetBitmapFile(filenameExt.String());
			
					if (bitmap)
						break;
				}
				if (bitmap)
					break;
			}
			if (bitmap == NULL)
				bitmap = BTranslationUtils::GetBitmapFile("defaultsnap.png");
			if (bitmap != NULL) {
				BRect viewBounds = fBitmapView->Bounds();
				BRect bitmapBounds = bitmap->Bounds();
				if (!Settings().StretchShots()) {
					viewBounds = bitmapBounds;
					center_rect_in_view(viewBounds, this);
				}
				fBitmapView->SetViewBitmap(bitmap, bitmapBounds, viewBounds, B_FOLLOW_NONE, 0);
			
				delete bitmap;
			}
			fBitmapView->Invalidate();
			break;
		}
		default:
			BView::MessageReceived(message);
			break;
	}
}


void
BitmapView::FrameResized(float new_width, float new_height)
{
	Window()->PostMessage(kTabChanged);
}

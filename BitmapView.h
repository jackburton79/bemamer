#ifndef __BITMAPVIEW_H
#define __BITMAPVIEW_H

#include <View.h>

class BitmapView : public BView
{
public:
	BitmapView(BRect rect);
	virtual void AttachedToWindow();
	virtual void MessageReceived(BMessage *message);
	virtual void FrameResized(float, float);
private:
	BView *fBitmapView;
};

#endif

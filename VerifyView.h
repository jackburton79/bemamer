#ifndef __VERIFYVIEW_H
#define __VERIFYVIEW_H

#include <TextView.h>

class BButton;
class VerifyView : public BView
{
public:
	VerifyView(BRect rect);
	virtual void AttachedToWindow();
	virtual void MessageReceived(BMessage *message);

private:
	BTextView *fReport;
	BButton *fButton;
};

#endif

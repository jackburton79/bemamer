#ifndef __INFOVIEW_H
#define __INFOVIEW_H

#include <View.h>

class BBox;
class BTextControl;
class InfoView : public BView
{
public:
	InfoView(BRect size);
	virtual void AttachedToWindow();
	virtual void MessageReceived(BMessage *message);	
private:
	BBox *fInfoBox;
	BTextControl *fTextControl[11];
};

#endif

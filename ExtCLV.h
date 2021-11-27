#ifndef __EXTCLV_H
#define __EXTCLV_H

#include <String.h>

#include "ColumnListView.h"

const uint32 kCheckTypeahead = 'Tcty';

class BPopUpMenu;
class TExtColumnListView : public BColumnListView
{
public:
	TExtColumnListView(BRect rect,
					const char *name,
					uint32 resizingMode,
					uint32 drawFlags,
					border_style = B_NO_BORDER,
					bool showHorizontalScrollbar = true);
	
	virtual void MessageReceived(BMessage *message);
	virtual void KeyDown(const char *bytes, int32 numBytes);
	virtual void MouseDown(BPoint where);
	//void SetMenu(BPopUpMenu *menu) { fPopupMenu = menu; }

private:	
	void SelectBestMatching();
	
	BString fBuffer;
	BMessageRunner *fKeyRunner;
	//uint32 fMatchingColumn;
	//BPopUpMenu *fPopupMenu;
	
	static bigtime_t sLastKeyTime;
	
};

#endif

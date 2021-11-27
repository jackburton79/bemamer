#ifndef __TTABVIEW_H
#define __TTABVIEW_H

#include <Invoker.h>
#include <TabView.h>

class TTabView : public BTabView, private BInvoker {
public:
	TTabView(BRect, const char*, button_width = B_WIDTH_AS_USUAL,
		uint32 resizingMode = B_FOLLOW_ALL, uint32 flags = B_FULL_UPDATE_ON_RESIZE |
		B_WILL_DRAW | B_NAVIGABLE_JUMP | B_FRAME_EVENTS | B_NAVIGABLE);
	
	virtual void Select(int32);
};


#endif

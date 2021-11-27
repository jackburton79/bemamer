#include "Messages.h"

#include "MyTabView.h"

#include <Window.h>

TTabView::TTabView(BRect rect, const char *name,
		button_width width, uint32 resizingMode, uint32 flags)
	:BTabView(rect, name, width, resizingMode, flags)
{
}


void
TTabView::Select(int32 tab)
{
	BMessage message(kTabChanged);
	SetTarget(Window());
	Invoke(&message);
	
	BTabView::Select(tab);
}

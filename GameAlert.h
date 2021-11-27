#ifndef __GAMEALERT_H
#define __GAMEALERT_H

#include <String.h>
#include <Window.h>

class BTextView;
class TGameAlert : public BWindow
{
public:
	TGameAlert(BString string);
	BTextView *TextView();
private:
	BTextView *fTextView;
};

#endif

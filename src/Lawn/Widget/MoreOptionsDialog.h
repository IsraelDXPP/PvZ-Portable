#ifndef __MOREOPTIONS_DIALOG_H__
#define __MOREOPTIONS_DIALOG_H__

#include "LawnDialog.h"
#include "widget/CheckboxListener.h"

class Checkbox;
class LawnStoneButton;

class MoreOptionsDialog : public LawnDialog, public Sexy::CheckboxListener
{
public:
	enum
	{
		MoreOptionsDialog_NoCrazyDaveSeeds,
		MoreOptionsDialog_AutoCollectSun,
		MoreOptionsDialog_AutoCollectCoins
	};

	Checkbox*				mNoCrazyDaveSeedsCheckbox;
	Checkbox*				mAutoCollectSunCheckbox;
	Checkbox*				mAutoCollectCoinsCheckbox;
	LawnStoneButton*		mBackButton;

public:
	MoreOptionsDialog(LawnApp* theApp);
	virtual ~MoreOptionsDialog();

	virtual void			CheckboxChecked(int theId, bool checked);
	virtual void			ButtonDepress(int theId);
	virtual void			KeyDown(KeyCode theKey);
	virtual void			Update();
	virtual void			Draw(Graphics* g);
	virtual void			AddedToManager(WidgetManager* theWidgetManager);
	virtual void			RemovedFromManager(WidgetManager* theWidgetManager);
};

#endif

#ifndef __MOREOPTIONS_DIALOG_H__
#define __MOREOPTIONS_DIALOG_H__

#include "../../GameConstants.h"
#include "LawnDialog.h"
#include "widget/CheckboxListener.h"


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

	Sexy::Checkbox*			mNoCrazyDaveSeedsCheckbox;
	Sexy::Checkbox*			mAutoCollectSunCheckbox;
	Sexy::Checkbox*			mAutoCollectCoinsCheckbox;
	LawnStoneButton*		mBackButton;

public:
	MoreOptionsDialog(LawnApp* theApp);
	virtual ~MoreOptionsDialog();

	virtual void			CheckboxChecked(int theId, bool checked);
	virtual void			ButtonDepress(int theId);
	virtual void			KeyDown(KeyCode theKey);
	virtual void			Update();
	virtual void			Resize(int theX, int theY, int theWidth, int theHeight);
	virtual void			Draw(Graphics* g);
	virtual void			AddedToManager(WidgetManager* theWidgetManager);
	virtual void			RemovedFromManager(WidgetManager* theWidgetManager);
};

#endif

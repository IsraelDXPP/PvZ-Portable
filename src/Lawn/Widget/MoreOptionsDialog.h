#ifndef __MOREOPTIONS_DIALOG_H__
#define __MOREOPTIONS_DIALOG_H__

#include "../../GameConstants.h"
#include "LawnDialog.h"
#include "widget/CheckboxListener.h"
#include "widget/ButtonWidget.h"

class LawnStoneButton;

class MoreOptionsDialog : public LawnDialog, public Sexy::CheckboxListener
{
public:
	enum
	{
		MoreOptionsDialog_NoCrazyDaveSeeds,
		MoreOptionsDialog_AutoCollectSun,
		MoreOptionsDialog_AutoCollectCoins,
		MoreOptionsDialog_UnlimitedSun,
		MoreOptionsDialog_NoCooldown,
		MoreOptionsDialog_PlantInColumns,
		MoreOptionsDialog_NoSunCost,
		MoreOptionsDialog_InvinciblePlants,
		MoreOptionsDialog_PlantAnywhere,
		MoreOptionsDialog_UnlockAll,
		MoreOptionsDialog_LevelSelector,
		MoreOptionsDialog_PrevPage,
		MoreOptionsDialog_NextPage
	};

	Sexy::Checkbox*			mNoCrazyDaveSeedsCheckbox;
	Sexy::Checkbox*			mAutoCollectSunCheckbox;
	Sexy::Checkbox*			mAutoCollectCoinsCheckbox;
	Sexy::Checkbox*			mUnlimitedSunCheckbox;
	Sexy::Checkbox*			mNoCooldownCheckbox;
	Sexy::Checkbox*			mPlantInColumnsCheckbox;
	Sexy::Checkbox*			mNoSunCostCheckbox;
	Sexy::Checkbox*			mInvinciblePlantsCheckbox;
	Sexy::Checkbox*			mPlantAnywhereCheckbox;
	LawnStoneButton*		mUnlockAllButton;
	Sexy::ButtonWidget*		mPrevButton;
	Sexy::ButtonWidget*		mNextButton;
	LawnStoneButton*		mLevelSelectorWidget;
	LawnStoneButton*		mBackButton;
	int						mCurrentPage;

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

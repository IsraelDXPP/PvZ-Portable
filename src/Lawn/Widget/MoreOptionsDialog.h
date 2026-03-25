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
		MoreOptionsDialog_AutoWin,
		MoreOptionsDialog_NoPlantCooldown,
		MoreOptionsDialog_RegenPlants,
		MoreOptionsDialog_ModMenuEnabled,
		MoreOptionsDialog_HypnotizeAll,
		MoreOptionsDialog_SummonZomboss,
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
	Sexy::Checkbox*			mAutoWinCheckbox;
	Sexy::Checkbox*			mNoPlantCooldownCheckbox;
	Sexy::Checkbox*			mRegenPlantsCheckbox;
	Sexy::Checkbox*			mModMenuEnabledCheckbox;
	LawnStoneButton*		mHypnotizeAllButton;
	LawnStoneButton*		mSummonZombossButton;
	LawnStoneButton*		mUnlockAllButton;
	Sexy::ButtonWidget*		mPrevButton;
	Sexy::ButtonWidget*		mNextButton;
	LawnStoneButton*		mLevelSelectorWidget;
	LawnStoneButton*		mBackButton;
	int						mCurrentPage;
	bool					mFromPauseMenu;

public:
	MoreOptionsDialog(LawnApp* theApp, bool theFromPauseMenu = false);
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

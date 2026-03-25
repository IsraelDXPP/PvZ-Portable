#include "MoreOptionsDialog.h"
#include "../../LawnApp.h"
#include "../../Resources.h"
#include "../LawnCommon.h"
#include "GameButton.h"
#include "widget/Checkbox.h"
#include "../../SexyAppFramework/widget/WidgetManager.h"
#include "../../SexyAppFramework/graphics/Color.h"
#include "../../Sexy.TodLib/TodStringFile.h"
#include "../../Sexy.TodLib/TodCommon.h"
#include "../../Sexy.TodLib/TodDebug.h"
#include "graphics/Font.h"
#include "graphics/Graphics.h"
#include "../System/PlayerInfo.h"

using namespace Sexy;

MoreOptionsDialog::MoreOptionsDialog(LawnApp* theApp) :
	LawnDialog(theApp, Dialogs::DIALOG_MORE_OPTIONS, true, "MORE OPTIONS", "", "", Dialog::BUTTONS_NONE)
{
	mApp = theApp;
	mDrawStandardBack = true;

	mNoCrazyDaveSeedsCheckbox = MakeNewCheckbox(MoreOptionsDialog_NoCrazyDaveSeeds, this, mApp->mPlayerInfo->mNoCrazyDaveSeeds);
	mAutoCollectSunCheckbox = MakeNewCheckbox(MoreOptionsDialog_AutoCollectSun, this, mApp->mPlayerInfo->mAutoCollectSun);
	mAutoCollectCoinsCheckbox = MakeNewCheckbox(MoreOptionsDialog_AutoCollectCoins, this, mApp->mPlayerInfo->mAutoCollectCoins);
	mUnlimitedSunCheckbox = MakeNewCheckbox(MoreOptionsDialog_UnlimitedSun, this, mApp->mPlayerInfo->mUnlimitedSun);
	mNoCooldownCheckbox = MakeNewCheckbox(MoreOptionsDialog_NoCooldown, this, mApp->mPlayerInfo->mNoCooldown);

	mPlantInColumnsCheckbox = MakeNewCheckbox(MoreOptionsDialog_PlantInColumns, this, mApp->mPlayerInfo->mPlantInColumns);
	mNoSunCostCheckbox = MakeNewCheckbox(MoreOptionsDialog_NoSunCost, this, mApp->mPlayerInfo->mNoSunCost);
	mInvinciblePlantsCheckbox = MakeNewCheckbox(MoreOptionsDialog_InvinciblePlants, this, mApp->mPlayerInfo->mInvinciblePlants);
	mPlantAnywhereCheckbox = MakeNewCheckbox(MoreOptionsDialog_PlantAnywhere, this, mApp->mPlayerInfo->mPlantAnywhere);
	mAutoWinCheckbox = MakeNewCheckbox(MoreOptionsDialog_AutoWin, this, mApp->mPlayerInfo->mAutoWin);

	mUnlockAllButton = MakeButton(MoreOptionsDialog_UnlockAll, this, "Unlock All!");
	mPrevButton = new Sexy::ButtonWidget(MoreOptionsDialog_PrevPage, this);
	mPrevButton->mFrameNoDraw = true;
	mNextButton = new Sexy::ButtonWidget(MoreOptionsDialog_NextPage, this);
	mNextButton->mFrameNoDraw = true;

	mLevelSelectorWidget = MakeButton(MoreOptionsDialog_LevelSelector, this, "Level Selector");

	mCurrentPage = 0;

	mBackButton = MakeButton(Dialog::ID_OK, this, "[DIALOG_BUTTON_OK]");

	CalcSize(110, 290);
}

MoreOptionsDialog::~MoreOptionsDialog()
{
	delete mNoCrazyDaveSeedsCheckbox;
	delete mAutoCollectSunCheckbox;
	delete mAutoCollectCoinsCheckbox;
	delete mUnlimitedSunCheckbox;
	delete mNoCooldownCheckbox;
	delete mPlantInColumnsCheckbox;
	delete mNoSunCostCheckbox;
	delete mInvinciblePlantsCheckbox;
	delete mPlantAnywhereCheckbox;
	delete mAutoWinCheckbox;
	delete mUnlockAllButton;
	delete mPrevButton;
	delete mNextButton;
	delete mLevelSelectorWidget;
	delete mBackButton;
}

void MoreOptionsDialog::CheckboxChecked(int theId, bool checked)
{
	mApp->PlaySample(SOUND_BUTTONCLICK);
	switch (theId)
	{
	case MoreOptionsDialog_NoCrazyDaveSeeds:
		mApp->mPlayerInfo->mNoCrazyDaveSeeds = checked;
		break;
	case MoreOptionsDialog_AutoCollectSun:
		mApp->mPlayerInfo->mAutoCollectSun = checked;
		break;
	case MoreOptionsDialog_AutoCollectCoins:
		mApp->mPlayerInfo->mAutoCollectCoins = checked;
		break;
	case MoreOptionsDialog_UnlimitedSun:
		mApp->mPlayerInfo->mUnlimitedSun = checked;
		break;
	case MoreOptionsDialog_NoCooldown:
		mApp->mPlayerInfo->mNoCooldown = checked;
		break;
	case MoreOptionsDialog_PlantInColumns:
		mApp->mPlayerInfo->mPlantInColumns = checked;
		break;
	case MoreOptionsDialog_NoSunCost:
		mApp->mPlayerInfo->mNoSunCost = checked;
		break;
	case MoreOptionsDialog_InvinciblePlants:
		mApp->mPlayerInfo->mInvinciblePlants = checked;
		break;
	case MoreOptionsDialog_PlantAnywhere:
		mApp->mPlayerInfo->mPlantAnywhere = checked;
		break;
	case MoreOptionsDialog_AutoWin:
		mApp->mPlayerInfo->mAutoWin = checked;
		break;
	}
}

void MoreOptionsDialog::ButtonDepress(int theId)
{
	if (theId == Dialog::ID_OK)
	{
		mApp->KillDialog(mId);
	}
	else if (theId == MoreOptionsDialog_LevelSelector)
	{
		mApp->PlaySample(SOUND_BUTTONCLICK);
		mApp->DoCheatDialog();
	}
	else if (theId == MoreOptionsDialog_PrevPage)
	{
		mApp->PlaySample(SOUND_BUTTONCLICK);
		if (mCurrentPage > 0) mCurrentPage--;
		Resize(mX, mY, mWidth, mHeight);
	}
	else if (theId == MoreOptionsDialog_NextPage)
	{
		mApp->PlaySample(SOUND_BUTTONCLICK);
		if (mCurrentPage < 1) mCurrentPage++;
		Resize(mX, mY, mWidth, mHeight);
	}
	else if (theId == MoreOptionsDialog_UnlockAll)
	{
		mApp->PlaySample(SOUND_BUTTONCLICK);
		mApp->mPlayerInfo->mFinishedAdventure = 1;
		mApp->mPlayerInfo->mLevel = 1;
		mApp->mPlayerInfo->mHasUnlockedMinigames = 1;
		mApp->mPlayerInfo->mHasUnlockedPuzzleMode = 1;
		mApp->mPlayerInfo->mHasUnlockedSurvivalMode = 1;
		mApp->mPlayerInfo->mHasNewMiniGame = 1;
		mApp->mPlayerInfo->mHasNewScaryPotter = 1;
		mApp->mPlayerInfo->mHasNewIZombie = 1;
		mApp->mPlayerInfo->mHasNewSurvival = 1;
		mApp->mPlayerInfo->AddCoins(999990); 
		for (int i = 0; i < 80; i++) {
			mApp->mPlayerInfo->mPurchases[i] = 1;
		}
		mApp->mPlayerInfo->mPurchases[0] = 3; // PACKET_UPGRADE
		for (int i = 0; i < 20; i++) {
			mApp->mPlayerInfo->mEarnedAchievements[i] = true;
			mApp->mPlayerInfo->mShownAchievements[i] = true;
		}
		mApp->mPlayerInfo->SaveDetails();
		mApp->KillDialog(mId);
	}
}

void MoreOptionsDialog::KeyDown(KeyCode theKey)
{
	if (theKey == KeyCode::KEYCODE_ESCAPE || theKey == KeyCode::KEYCODE_RETURN || theKey == KeyCode::KEYCODE_SPACE)
	{
		ButtonDepress(Dialog::ID_OK);
	}
}

void MoreOptionsDialog::Update()
{
	LawnDialog::Update();
}

void MoreOptionsDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
	LawnDialog::Resize(theX, theY, theWidth, theHeight);

	int aViewX = 50;
	int aViewY = 110;
	int aStepY = 32;

	mNoCrazyDaveSeedsCheckbox->SetVisible(mCurrentPage == 0 && mApp->HasFinishedAdventure());
	mAutoCollectSunCheckbox->SetVisible(mCurrentPage == 0);
	mAutoCollectCoinsCheckbox->SetVisible(mCurrentPage == 0);
	mUnlimitedSunCheckbox->SetVisible(mCurrentPage == 0);
	mNoCooldownCheckbox->SetVisible(mCurrentPage == 0);

	mPlantInColumnsCheckbox->SetVisible(mCurrentPage == 1);
	mNoSunCostCheckbox->SetVisible(mCurrentPage == 1);
	mInvinciblePlantsCheckbox->SetVisible(mCurrentPage == 1);
	mPlantAnywhereCheckbox->SetVisible(mCurrentPage == 1);
	mUnlockAllButton->SetVisible(mCurrentPage == 1);
	mLevelSelectorWidget->SetVisible(mCurrentPage == 1);

	if (mCurrentPage == 0)
	{
		if (mApp->HasFinishedAdventure())
		{
			mNoCrazyDaveSeedsCheckbox->Resize(aViewX, aViewY, 46, 45);
			aViewY += aStepY;
		}
		mAutoCollectSunCheckbox->Resize(aViewX, aViewY, 46, 45); aViewY += aStepY;
		mAutoCollectCoinsCheckbox->Resize(aViewX, aViewY, 46, 45); aViewY += aStepY;
		mUnlimitedSunCheckbox->Resize(aViewX, aViewY, 46, 45); aViewY += aStepY;
		mNoCooldownCheckbox->Resize(aViewX, aViewY, 46, 45);
	}
	else if (mCurrentPage == 1)
	{
		mPlantInColumnsCheckbox->Resize(aViewX, aViewY, 46, 45); aViewY += aStepY;
		mNoSunCostCheckbox->Resize(aViewX, aViewY, 46, 45); aViewY += aStepY;
		mInvinciblePlantsCheckbox->Resize(aViewX, aViewY, 46, 45); aViewY += aStepY;
		mPlantAnywhereCheckbox->Resize(aViewX, aViewY, 46, 45); aViewY += aStepY;
		mAutoWinCheckbox->Resize(aViewX, aViewY, 46, 45); aViewY += aStepY;

		mLevelSelectorWidget->Resize(aViewX - 10, aViewY + 5, 209, 46);
		aViewY += 46 + 10;
		mUnlockAllButton->Resize(aViewX - 10, aViewY + 5, 209, 46);
	}

	mBackButton->Resize(theWidth / 2 - 104, theHeight - 65, 209, 46);
	
	mPrevButton->SetVisible(mCurrentPage > 0);
	mNextButton->SetVisible(mCurrentPage < 1);

	int aArrowWidth = IMAGE_ZEN_NEXTGARDEN->mWidth;
	int aEdgeOffset = 60; // La punta de la flecha quedará a 20px del borde (20 + 40)

	mPrevButton->Resize(60 - aArrowWidth, theHeight - 80, aArrowWidth, 40);
	mNextButton->Resize(theWidth - 60, theHeight - 80, aArrowWidth, 40);
}

void MoreOptionsDialog::Draw(Graphics* g)
{
	LawnDialog::Draw(g);

	Color aTextColor(107, 109, 145);
	int aLabelX = 100;

	if (mCurrentPage == 0)
	{
		if (mApp->HasFinishedAdventure())
		{
			TodDrawString(g, "No Crazy Dave Seeds", aLabelX, mNoCrazyDaveSeedsCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
		}
		TodDrawString(g, "Auto Collect Sun", aLabelX, mAutoCollectSunCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
		TodDrawString(g, "Auto Collect Coins", aLabelX, mAutoCollectCoinsCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
		TodDrawString(g, "Unlimited Sun", aLabelX, mUnlimitedSunCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
		TodDrawString(g, "No More Cooldown", aLabelX, mNoCooldownCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
	}
	else if (mCurrentPage == 1)
	{
		TodDrawString(g, "Plant in Columns", aLabelX, mPlantInColumnsCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
		TodDrawString(g, "No Sun Cost", aLabelX, mNoSunCostCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
		TodDrawString(g, "Invincible Plants", aLabelX, mInvinciblePlantsCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
		TodDrawString(g, "Plant Anywhere", aLabelX, mPlantAnywhereCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
		TodDrawString(g, "Auto Win", aLabelX, mAutoWinCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
	}

	TodDrawString(g, StrFormat("%d / 2", mCurrentPage + 1), mWidth / 2, mHeight - 75, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_CENTER);

	int aPrevOffsetY = mPrevButton->mIsDown ? 1 : 0;
	if (mCurrentPage > 0)
	{
		g->DrawImageMirror(IMAGE_ZEN_NEXTGARDEN, mPrevButton->mX, mPrevButton->mY + aPrevOffsetY, true);
	}

	int aNextOffsetY = mNextButton->mIsDown ? 1 : 0;
	if (mCurrentPage < 1)
	{
		g->DrawImage(IMAGE_ZEN_NEXTGARDEN, mNextButton->mX, mNextButton->mY + aNextOffsetY);
	}
}

void MoreOptionsDialog::AddedToManager(WidgetManager* theWidgetManager)
{
	LawnDialog::AddedToManager(theWidgetManager);
	AddWidget(mNoCrazyDaveSeedsCheckbox);
	AddWidget(mAutoCollectSunCheckbox);
	AddWidget(mAutoCollectCoinsCheckbox);
	AddWidget(mUnlimitedSunCheckbox);
	AddWidget(mNoCooldownCheckbox);
	AddWidget(mPlantInColumnsCheckbox);
	AddWidget(mNoSunCostCheckbox);
	AddWidget(mInvinciblePlantsCheckbox);
	AddWidget(mPlantAnywhereCheckbox);
	AddWidget(mAutoWinCheckbox);
	AddWidget(mUnlockAllButton);
	AddWidget(mPrevButton);
	AddWidget(mNextButton);
	AddWidget(mLevelSelectorWidget);
	AddWidget(mBackButton);
}

void MoreOptionsDialog::RemovedFromManager(WidgetManager* theWidgetManager)
{
	LawnDialog::RemovedFromManager(theWidgetManager);
	RemoveWidget(mNoCrazyDaveSeedsCheckbox);
	RemoveWidget(mAutoCollectSunCheckbox);
	RemoveWidget(mAutoCollectCoinsCheckbox);
	RemoveWidget(mUnlimitedSunCheckbox);
	RemoveWidget(mNoCooldownCheckbox);
	RemoveWidget(mPlantInColumnsCheckbox);
	RemoveWidget(mNoSunCostCheckbox);
	RemoveWidget(mInvinciblePlantsCheckbox);
	RemoveWidget(mPlantAnywhereCheckbox);
	RemoveWidget(mAutoWinCheckbox);
	RemoveWidget(mUnlockAllButton);
	RemoveWidget(mPrevButton);
	RemoveWidget(mNextButton);
	RemoveWidget(mLevelSelectorWidget);
	RemoveWidget(mBackButton);
}

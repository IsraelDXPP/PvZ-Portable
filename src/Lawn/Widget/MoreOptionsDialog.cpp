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
#include "../Board.h"
#include "../Zombie.h"
#include "../Cutscene.h"

using namespace Sexy;

MoreOptionsDialog::MoreOptionsDialog(LawnApp* theApp, bool theFromPauseMenu) :
	LawnDialog(theApp, Dialogs::DIALOG_MORE_OPTIONS, true, "MORE OPTIONS", "", "", Dialog::BUTTONS_NONE)
{
	mApp = theApp;
	mFromPauseMenu = theFromPauseMenu;
	mDrawStandardBack = true;
	mModMenuEnabledCheckbox = nullptr;
	mHypnotizeAllButton = nullptr;

	// Ensure Credits resources (IMAGE_CREDITS_PLAYBUTTON, etc.) are loaded on demand
	if (!mApp->mResourceManager->IsGroupLoaded("DelayLoad_Credits"))
	{
		mApp->mResourceManager->LoadResources("DelayLoad_Credits");
		Sexy::ExtractDelayLoad_CreditsResources(mApp->mResourceManager);
	}

	if (!mFromPauseMenu)
	{
		mModMenuEnabledCheckbox = MakeNewCheckbox(MoreOptionsDialog_ModMenuEnabled, this, mApp->mPlayerInfo->mModMenuEnabled);
	}
	else
	{
		mHypnotizeAllButton = MakeButton(MoreOptionsDialog_HypnotizeAll, this, "Hypnotize All");
	}

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
	mNoPlantCooldownCheckbox = MakeNewCheckbox(MoreOptionsDialog_NoPlantCooldown, this, mApp->mPlayerInfo->mNoPlantCooldown);
	mRegenPlantsCheckbox = MakeNewCheckbox(MoreOptionsDialog_RegenPlants, this, mApp->mPlayerInfo->mRegenPlants);

	mUnlockAllButton = MakeButton(MoreOptionsDialog_UnlockAll, this, "Unlock All!");
	mSpawnMenuButton = MakeButton(MoreOptionsDialog_SpawnMenu, this, "Spawn Menu (WIP)");
	mDebugInfoCheckbox = MakeNewCheckbox(MoreOptionsDialog_DebugInfo, this, mApp->mPlayerInfo->mDebugInfo);
	mNoCraterCheckbox = MakeNewCheckbox(MoreOptionsDialog_NoCrater, this, mApp->mPlayerInfo->mNoCrater);
	mKillAllButton = MakeButton(MoreOptionsDialog_KillAll, this, "Kill All Zombies");
	mFreezeAllButton = MakeButton(MoreOptionsDialog_FreezeAll, this, "Freeze All Zombies");
	mBurnAllButton = MakeButton(MoreOptionsDialog_BurnAll, this, "Burn All Zombies");

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
	delete mNoPlantCooldownCheckbox;
	delete mRegenPlantsCheckbox;
	delete mModMenuEnabledCheckbox;
	delete mHypnotizeAllButton;
	delete mUnlockAllButton;
	delete mDebugInfoCheckbox;
	delete mNoCraterCheckbox;
	delete mSpawnMenuButton;
	delete mKillAllButton;
	delete mFreezeAllButton;
	delete mBurnAllButton;
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
	case MoreOptionsDialog_NoPlantCooldown:
		mApp->mPlayerInfo->mNoPlantCooldown = checked;
		break;
	case MoreOptionsDialog_RegenPlants:
		mApp->mPlayerInfo->mRegenPlants = checked;
		break;
	case MoreOptionsDialog_ModMenuEnabled:
		mApp->mPlayerInfo->mModMenuEnabled = checked;
		break;
	case MoreOptionsDialog_DebugInfo:
		mApp->mPlayerInfo->mDebugInfo = checked;
		break;
	case MoreOptionsDialog_NoCrater:
		mApp->mPlayerInfo->mNoCrater = checked;
		break;
	}
	mApp->mPlayerInfo->SaveCheats();
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
	else if (theId == MoreOptionsDialog_HypnotizeAll)
	{
		mApp->PlaySample(SOUND_BUTTONCLICK);
		if (mApp->mBoard)
		{
			Zombie* aZombie = nullptr;
			while (mApp->mBoard->IterateZombies(aZombie))
			{
				if (!aZombie->mDead && !aZombie->mMindControlled)
				{
					aZombie->StartMindControlled();
				}
			}
		}
		mApp->KillDialog(mId);
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
		if (mCurrentPage < 2) mCurrentPage++;
		Resize(mX, mY, mWidth, mHeight);
	}
	else if (theId == MoreOptionsDialog_KillAll)
	{
		mApp->PlaySample(SOUND_BUTTONCLICK);
		if (mApp->mBoard)
		{
			Zombie* aZombie = nullptr;
			while (mApp->mBoard->IterateZombies(aZombie))
			{
				if (!aZombie->IsDeadOrDying()) aZombie->DieNoLoot();
			}
		}
	}
	else if (theId == MoreOptionsDialog_FreezeAll)
	{
		mApp->PlaySample(SOUND_BUTTONCLICK);
		if (mApp->mBoard)
		{
			Zombie* aZombie = nullptr;
			while (mApp->mBoard->IterateZombies(aZombie))
			{
				if (!aZombie->IsDeadOrDying()) aZombie->ApplyChill(true);
			}
		}
	}
	else if (theId == MoreOptionsDialog_BurnAll)
	{
		mApp->PlaySample(SOUND_BUTTONCLICK);
		if (mApp->mBoard)
		{
			Zombie* aZombie = nullptr;
			while (mApp->mBoard->IterateZombies(aZombie))
			{
				if (!aZombie->IsDeadOrDying()) aZombie->ApplyBurn();
			}
		}
	}
	else if (theId == MoreOptionsDialog_SpawnMenu)
	{
		mApp->PlaySample(SOUND_BUTTONCLICK);
		mApp->DoSpawnZombieDialog();
	}
	else if (theId == MoreOptionsDialog_UnlockAll)
	{
		mApp->PlaySample(SOUND_BUTTONCLICK);
		mApp->mPlayerInfo->mLevel = 1;
		mApp->mPlayerInfo->mFinishedAdventure = 2;
		mApp->mPlayerInfo->mHasUnlockedMinigames = 1;
		mApp->mPlayerInfo->mHasUnlockedPuzzleMode = 1;
		mApp->mPlayerInfo->mHasUnlockedSurvivalMode = 1;
		mApp->mPlayerInfo->mHasWokenStinky = 1;
		mApp->mPlayerInfo->mHasSeenStinky = 1;
		mApp->mPlayerInfo->mHasNewScaryPotter = 1;
		mApp->mPlayerInfo->mHasNewIZombie = 1;
		mApp->mPlayerInfo->mHasNewSurvival = 1;
		mApp->mPlayerInfo->AddCoins(999990); 
		for (int i = 0; i < 100; i++) {
			mApp->mPlayerInfo->mChallengeRecords[i] = 20;
		}
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

	// Page 1 Visibility
	mAutoCollectSunCheckbox->SetVisible(mCurrentPage == 0);
	mAutoCollectCoinsCheckbox->SetVisible(mCurrentPage == 0);
	mUnlimitedSunCheckbox->SetVisible(mCurrentPage == 0);
	mNoSunCostCheckbox->SetVisible(mCurrentPage == 0);
	mNoCooldownCheckbox->SetVisible(mCurrentPage == 0);
	if (mNoCrazyDaveSeedsCheckbox) mNoCrazyDaveSeedsCheckbox->SetVisible(mCurrentPage == 0 && !mFromPauseMenu && mApp->HasFinishedAdventure());
	if (mModMenuEnabledCheckbox) mModMenuEnabledCheckbox->SetVisible(mCurrentPage == 0 && !mFromPauseMenu);

	// Page 2 Visibility
	mInvinciblePlantsCheckbox->SetVisible(mCurrentPage == 1);
	mRegenPlantsCheckbox->SetVisible(mCurrentPage == 1);
	mPlantAnywhereCheckbox->SetVisible(mCurrentPage == 1);
	mPlantInColumnsCheckbox->SetVisible(mCurrentPage == 1);
	mNoPlantCooldownCheckbox->SetVisible(mCurrentPage == 1);
	mAutoWinCheckbox->SetVisible(mCurrentPage == 1);
	mLevelSelectorWidget->SetVisible(mCurrentPage == 1);
	mUnlockAllButton->SetVisible(mCurrentPage == 1 && !mFromPauseMenu);

	// Page 3 Visibility
	mDebugInfoCheckbox->SetVisible(mCurrentPage == 2);
	mNoCraterCheckbox->SetVisible(mCurrentPage == 2);
	mSpawnMenuButton->SetVisible(mCurrentPage == 2 && mFromPauseMenu);
	mKillAllButton->SetVisible(mCurrentPage == 2 && mFromPauseMenu);
	mFreezeAllButton->SetVisible(mCurrentPage == 2 && mFromPauseMenu);
	mBurnAllButton->SetVisible(mCurrentPage == 2 && mFromPauseMenu);
	if (mHypnotizeAllButton) mHypnotizeAllButton->SetVisible(mCurrentPage == 2 && mFromPauseMenu);

	if (mCurrentPage == 0)
	{
		mAutoCollectSunCheckbox->Resize(aViewX, aViewY, 46, 45); aViewY += aStepY;
		mAutoCollectCoinsCheckbox->Resize(aViewX, aViewY, 46, 45); aViewY += aStepY;
		mUnlimitedSunCheckbox->Resize(aViewX, aViewY, 46, 45); aViewY += aStepY;
		mNoSunCostCheckbox->Resize(aViewX, aViewY, 46, 45); aViewY += aStepY;
		mNoCooldownCheckbox->Resize(aViewX, aViewY, 46, 45); aViewY += aStepY;
		if (mApp->HasFinishedAdventure())
		{
			mNoCrazyDaveSeedsCheckbox->Resize(aViewX, aViewY, 46, 45); aViewY += aStepY;
		}

		if (!mFromPauseMenu && mModMenuEnabledCheckbox)
		{
			mModMenuEnabledCheckbox->Resize(aViewX, aViewY, 46, 45);
		}
	}
	else if (mCurrentPage == 1)
	{
		mInvinciblePlantsCheckbox->Resize(aViewX, aViewY, 46, 45); aViewY += aStepY;
		mRegenPlantsCheckbox->Resize(aViewX, aViewY, 46, 45); aViewY += aStepY;
		mNoPlantCooldownCheckbox->Resize(aViewX, aViewY, 46, 45); aViewY += aStepY;
		mPlantAnywhereCheckbox->Resize(aViewX, aViewY, 46, 45); aViewY += aStepY;
		mPlantInColumnsCheckbox->Resize(aViewX, aViewY, 46, 45); aViewY += aStepY;
		mAutoWinCheckbox->Resize(aViewX, aViewY, 46, 45); aViewY += aStepY;

		mLevelSelectorWidget->Resize(aViewX - 10, aViewY + 5, 209, 46);
		aViewY += 46 + 10;
		mUnlockAllButton->Resize(aViewX - 10, aViewY + 5, 209, 46);
	}
	else if (mCurrentPage == 2)
	{
		mDebugInfoCheckbox->Resize(aViewX, aViewY, 46, 45); aViewY += aStepY;
		mNoCraterCheckbox->Resize(aViewX, aViewY, 46, 45); aViewY += aStepY;
		
		if (mFromPauseMenu)
		{
			mSpawnMenuButton->Resize(aViewX - 10, aViewY + 5, 209, 42); aViewY += 42 + 5;
			mKillAllButton->Resize(aViewX - 10, aViewY + 5, 209, 42); aViewY += 42 + 5;
			mFreezeAllButton->Resize(aViewX - 10, aViewY + 5, 209, 42); aViewY += 42 + 5;
			mBurnAllButton->Resize(aViewX - 10, aViewY + 5, 209, 42); aViewY += 42 + 5;
			if (mHypnotizeAllButton) mHypnotizeAllButton->Resize(aViewX - 10, aViewY + 5, 209, 42); 
		}
	}

	mBackButton->Resize(theWidth / 2 - 104, theHeight - 65, 209, 46);
	
	mPrevButton->SetVisible(mCurrentPage > 0);
	mNextButton->SetVisible(mCurrentPage < 2);

	int aArrowWidth = IMAGE_ZEN_NEXTGARDEN ? IMAGE_ZEN_NEXTGARDEN->mWidth : 45;
	int aMargin = 20;

	mPrevButton->Resize(10, theHeight - 80, aArrowWidth, 40);
	mNextButton->Resize(theWidth - aMargin - aArrowWidth, theHeight - 80, aArrowWidth, 40);
}

void MoreOptionsDialog::Draw(Graphics* g)
{
	LawnDialog::Draw(g);

	Color aTextColor(107, 109, 145);
	int aLabelX = 100;

	if (mCurrentPage == 0)
	{
		TodDrawString(g, "Auto Collect Sun", aLabelX, mAutoCollectSunCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
		TodDrawString(g, "Auto Collect Coins", aLabelX, mAutoCollectCoinsCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
		TodDrawString(g, "Unlimited Sun", aLabelX, mUnlimitedSunCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
		TodDrawString(g, "No Sun Cost", aLabelX, mNoSunCostCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
		TodDrawString(g, "No Seed Cooldown", aLabelX, mNoCooldownCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
		
		if (mApp->HasFinishedAdventure() && !mFromPauseMenu && mNoCrazyDaveSeedsCheckbox)
		{
			TodDrawString(g, "No Crazy Dave Seeds", aLabelX, mNoCrazyDaveSeedsCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
		}

		if (!mFromPauseMenu && mModMenuEnabledCheckbox)
		{
			TodDrawString(g, "Mod Menu Enabled", aLabelX, mModMenuEnabledCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
		}
	}
	else if (mCurrentPage == 1)
	{
		TodDrawString(g, "Invincible Plants", aLabelX, mInvinciblePlantsCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
		TodDrawString(g, "Plant Regeneration", aLabelX, mRegenPlantsCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
		TodDrawString(g, "No Plant Cooldown", aLabelX, mNoPlantCooldownCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
		TodDrawString(g, "Plant Anywhere", aLabelX, mPlantAnywhereCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
		TodDrawString(g, "Plant in Columns", aLabelX, mPlantInColumnsCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
		TodDrawString(g, "Auto Win Level", aLabelX, mAutoWinCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
	}
	else if (mCurrentPage == 2)
	{
		TodDrawString(g, "Show Debug Info", aLabelX, mDebugInfoCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
		TodDrawString(g, "No Crater", aLabelX, mNoCraterCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
	}

	TodDrawString(g, StrFormat("%d / 3", mCurrentPage + 1), mWidth / 2, mHeight - 75, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_CENTER);

	int aPrevOffsetY = mPrevButton->mIsDown ? 1 : 0;
	if (mCurrentPage > 0 && IMAGE_ZEN_NEXTGARDEN)
	{
		g->DrawImageMirror(
			IMAGE_ZEN_NEXTGARDEN,
			mPrevButton->mX,
			mPrevButton->mY + aPrevOffsetY,
			true
		);
	}

	int aNextOffsetY = mNextButton->mIsDown ? 1 : 0;
	if (mCurrentPage < 2 && IMAGE_ZEN_NEXTGARDEN)
	{
		g->DrawImage(
			IMAGE_ZEN_NEXTGARDEN,
			mNextButton->mX,
			mNextButton->mY + aNextOffsetY
		);
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
	AddWidget(mNoPlantCooldownCheckbox);
	AddWidget(mRegenPlantsCheckbox);
	if (mModMenuEnabledCheckbox) AddWidget(mModMenuEnabledCheckbox);
	if (mHypnotizeAllButton) AddWidget(mHypnotizeAllButton);
	AddWidget(mUnlockAllButton);
	AddWidget(mDebugInfoCheckbox);
	AddWidget(mNoCraterCheckbox);
	AddWidget(mSpawnMenuButton);
	AddWidget(mKillAllButton);
	AddWidget(mFreezeAllButton);
	AddWidget(mBurnAllButton);
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
	RemoveWidget(mNoPlantCooldownCheckbox);
	RemoveWidget(mRegenPlantsCheckbox);
	if (mModMenuEnabledCheckbox) RemoveWidget(mModMenuEnabledCheckbox);
	if (mHypnotizeAllButton) RemoveWidget(mHypnotizeAllButton);
	RemoveWidget(mUnlockAllButton);
	RemoveWidget(mDebugInfoCheckbox);
	RemoveWidget(mNoCraterCheckbox);
	RemoveWidget(mSpawnMenuButton);
	RemoveWidget(mKillAllButton);
	RemoveWidget(mFreezeAllButton);
	RemoveWidget(mBurnAllButton);
	RemoveWidget(mPrevButton);
	RemoveWidget(mNextButton);
	RemoveWidget(mLevelSelectorWidget);
	RemoveWidget(mBackButton);
}

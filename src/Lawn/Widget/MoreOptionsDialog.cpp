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
	}
}

void MoreOptionsDialog::ButtonDepress(int theId)
{
	if (theId == Dialog::ID_OK)
	{
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
	int aViewY = 100; // Reset to 100 to prevent overlapping the title
	int aStepY = 40;

	if (mApp->HasFinishedAdventure())
	{
		mNoCrazyDaveSeedsCheckbox->Resize(aViewX, aViewY, 46, 45);
		mNoCrazyDaveSeedsCheckbox->SetVisible(true);
		aViewY += aStepY;
	}
	else
	{
		mNoCrazyDaveSeedsCheckbox->SetVisible(false);
	}

	mAutoCollectSunCheckbox->Resize(aViewX, aViewY, 46, 45);
	aViewY += aStepY;
	mAutoCollectCoinsCheckbox->Resize(aViewX, aViewY, 46, 45);
	aViewY += aStepY;
	mUnlimitedSunCheckbox->Resize(aViewX, aViewY, 46, 45);
	aViewY += aStepY;
	mNoCooldownCheckbox->Resize(aViewX, aViewY, 46, 45);

	mBackButton->Resize(theWidth / 2 - 104, theHeight - 65, 209, 46);
}

void MoreOptionsDialog::Draw(Graphics* g)
{
	LawnDialog::Draw(g);

	Color aTextColor(107, 109, 145);
	int aLabelX = 100;

	if (mApp->HasFinishedAdventure())
	{
		TodDrawString(g, "No Crazy Dave Seeds", aLabelX, mNoCrazyDaveSeedsCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
	}
	TodDrawString(g, "Auto Collect Sun", aLabelX, mAutoCollectSunCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
	TodDrawString(g, "Auto Collect Coins", aLabelX, mAutoCollectCoinsCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
	TodDrawString(g, "Unlimited Sun", aLabelX, mUnlimitedSunCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
	TodDrawString(g, "No More Cooldown", aLabelX, mNoCooldownCheckbox->mY + 28, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
}

void MoreOptionsDialog::AddedToManager(WidgetManager* theWidgetManager)
{
	LawnDialog::AddedToManager(theWidgetManager);
	AddWidget(mNoCrazyDaveSeedsCheckbox);
	AddWidget(mAutoCollectSunCheckbox);
	AddWidget(mAutoCollectCoinsCheckbox);
	AddWidget(mUnlimitedSunCheckbox);
	AddWidget(mNoCooldownCheckbox);
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
	RemoveWidget(mBackButton);
}

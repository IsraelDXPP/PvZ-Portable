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

	mBackButton = MakeButton(Dialog::ID_OK, this, "[DIALOG_BUTTON_OK]");

	CalcSize(110, 150);
}

MoreOptionsDialog::~MoreOptionsDialog()
{
	delete mNoCrazyDaveSeedsCheckbox;
	delete mAutoCollectSunCheckbox;
	delete mAutoCollectCoinsCheckbox;
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

	int aViewX = 70;
	int aViewY = 100;
	int aStepY = 45;

	mNoCrazyDaveSeedsCheckbox->Move(aViewX, aViewY);
	mAutoCollectSunCheckbox->Move(aViewX, aViewY + aStepY);
	mAutoCollectCoinsCheckbox->Move(aViewX, aViewY + aStepY * 2);

	mBackButton->Resize(theWidth / 2 - 50, theHeight - 50, 100, 33);
}

void MoreOptionsDialog::Draw(Graphics* g)
{
	LawnDialog::Draw(g);

	Color aTextColor(107, 109, 145);
	int aLabelX = 110;
	int aBaseY = 120;
	int aStepY = 45;

	TodDrawString(g, "No Crazy Dave Seeds", aLabelX, aBaseY, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
	TodDrawString(g, "Auto Collect Sun", aLabelX, aBaseY + aStepY, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
	TodDrawString(g, "Auto Collect Coins", aLabelX, aBaseY + aStepY * 2, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
}

void MoreOptionsDialog::AddedToManager(WidgetManager* theWidgetManager)
{
	LawnDialog::AddedToManager(theWidgetManager);
	AddWidget(mNoCrazyDaveSeedsCheckbox);
	AddWidget(mAutoCollectSunCheckbox);
	AddWidget(mAutoCollectCoinsCheckbox);
	AddWidget(mBackButton);
}

void MoreOptionsDialog::RemovedFromManager(WidgetManager* theWidgetManager)
{
	LawnDialog::RemovedFromManager(theWidgetManager);
	RemoveWidget(mNoCrazyDaveSeedsCheckbox);
	RemoveWidget(mAutoCollectSunCheckbox);
	RemoveWidget(mAutoCollectCoinsCheckbox);
	RemoveWidget(mBackButton);
}

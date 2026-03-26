#include "SpawnZombieDialog.h"
#include "../../LawnApp.h"
#include "../Board.h"
#include "../Zombie.h"
#include "../../Resources.h"
#include "../LawnCommon.h"
#include "GameButton.h"
#include "widget/Slider.h"
#include "widget/Checkbox.h"
#include "../../Sexy.TodLib/TodFoley.h"

using namespace Sexy;

SpawnZombieDialog::SpawnZombieDialog(LawnApp* theApp) :
	LawnDialog(theApp, Dialogs::DIALOG_SPAWN_ZOMBIE, true, "SPAWN MENU", "", "", Dialog::BUTTONS_NONE)
{
	mApp = theApp;
	mSelectedType = ZOMBIE_NORMAL;
	mSelectedRow = 0;
	mSelectedCol = 0;
	mIsHypnotized = false;

	mCloseButton = MakeButton(SpawnZombieDialog_Close, this, "[CLOSE_BUTTON]");
	mSpawnButton = MakeButton(SpawnZombieDialog_Spawn, this, "SPAWN!");
	
	mTypeSlider = new Slider(IMAGE_OPTIONS_SLIDERSLOT, IMAGE_OPTIONS_SLIDERKNOB2, SpawnZombieDialog_TypeSlider, this);
	mTypeSlider->SetValue((double)mSelectedType / (NUM_ZOMBIE_TYPES - 1));

	mRowSlider = new Slider(IMAGE_OPTIONS_SLIDERSLOT, IMAGE_OPTIONS_SLIDERKNOB2, SpawnZombieDialog_RowSlider, this);
	mRowSlider->SetValue(0);

	mColSlider = new Slider(IMAGE_OPTIONS_SLIDERSLOT, IMAGE_OPTIONS_SLIDERKNOB2, SpawnZombieDialog_ColSlider, this);
	mColSlider->SetValue(0);

	mHypnotizedCheckbox = MakeNewCheckbox(SpawnZombieDialog_Hypnotized, this, false);

	AddWidget(mCloseButton);
	AddWidget(mSpawnButton);
	AddWidget(mTypeSlider);
	AddWidget(mRowSlider);
	AddWidget(mColSlider);
	AddWidget(mHypnotizedCheckbox);

	Resize(0, 0, 600, 520);
	
	mCloseButton->Resize(120, 440, 150, 33);
	mSpawnButton->Resize(330, 440, 150, 33);
	
	mTypeSlider->Resize(50, 120, 500, 30);
	mRowSlider->Resize(150, 200, 400, 30);
	mColSlider->Resize(150, 260, 400, 30);
	mHypnotizedCheckbox->Resize(50, 320, 46, 45);
}

SpawnZombieDialog::~SpawnZombieDialog()
{
	RemoveWidget(mTypeSlider);
	RemoveWidget(mRowSlider);
	RemoveWidget(mColSlider);
	RemoveWidget(mHypnotizedCheckbox);
	RemoveWidget(mCloseButton);
	RemoveWidget(mSpawnButton);
	delete mTypeSlider;
	delete mRowSlider;
	delete mColSlider;
	delete mHypnotizedCheckbox;
	delete mCloseButton;
	delete mSpawnButton;
}

void SpawnZombieDialog::Draw(Graphics* g)
{
	LawnDialog::Draw(g);
	
	g->SetFont(FONT_PICO129);
	g->SetColor(Color::White);
	
	std::string aTypeName = "Zombie: " + std::string(gZombieDefs[mSelectedType].mZombieName);
	g->DrawString(aTypeName, 50, 110);
	
	g->DrawString("Row: " + std::to_string(mSelectedRow), 50, 222);
	g->DrawString("Col: " + std::to_string(mSelectedCol), 50, 282);
	g->DrawString("Hypnotize Zombie (Ally)", 110, 352);

	if (mApp->mBoard)
	{
		std::string aRowLimit = mApp->mBoard->StageHas6Rows() ? "(0-5)" : "(0-4)";
		g->DrawString(aRowLimit, 50, 238);
	}
}

void SpawnZombieDialog::Update()
{
	LawnDialog::Update();
}

void SpawnZombieDialog::SliderVal(int theId, double theVal)
{
	if (theId == SpawnZombieDialog_TypeSlider)
	{
		mSelectedType = (ZombieType)(int)(theVal * (NUM_ZOMBIE_TYPES - 1) + 0.5);
	}
	else if (theId == SpawnZombieDialog_RowSlider)
	{
		int aMaxRow = (mApp->mBoard && mApp->mBoard->StageHas6Rows()) ? 5 : 4;
		mSelectedRow = (int)(theVal * aMaxRow + 0.5);
	}
	else if (theId == SpawnZombieDialog_ColSlider)
	{
		mSelectedCol = (int)(theVal * 8 + 0.5); // 0-8
	}
}

void SpawnZombieDialog::CheckboxChecked(int theId, bool checked)
{
	if (theId == SpawnZombieDialog_Hypnotized)
	{
		mIsHypnotized = checked;
	}
}

void SpawnZombieDialog::ButtonDepress(int theId)
{
	if (theId == SpawnZombieDialog_Close)
	{
		mApp->KillSpawnZombieDialog();
	}
	else if (theId == SpawnZombieDialog_Spawn)
	{
		mApp->PlaySample(SOUND_BUTTONCLICK);
		if (mApp->mBoard)
		{
			// Verify row is valid for current stage
			int aMaxRow = mApp->mBoard->StageHas6Rows() ? 5 : 4;
			if (mSelectedRow > aMaxRow) mSelectedRow = aMaxRow;

			Zombie* aZombie = mApp->mBoard->AddZombieInRow(mSelectedType, mSelectedRow, 0);
			if (aZombie)
			{
				float aX = (float)mApp->mBoard->GridToPixelX(mSelectedCol, mSelectedRow);
				aZombie->mPosX = aX;
				aZombie->mX = aX;
				
				// Fix Y position for roof/slopes
				aZombie->mPosY = aZombie->GetPosYBasedOnRow(mSelectedRow);
				aZombie->mY = aZombie->mPosY;

				if (mIsHypnotized) aZombie->StartMindControlled();
			}
		}
	}
}

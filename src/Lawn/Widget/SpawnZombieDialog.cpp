#include "SpawnZombieDialog.h"
#include "../../LawnApp.h"
#include "../Board.h"
#include "../Zombie.h"
#include "../../Resources.h"
#include "../LawnCommon.h"
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
	
	mTypeSlider = new Slider(IMAGE_OPTIONS_SLIDERSLOT, IMAGE_OPTIONS_SLIDERTHUMB, SpawnZombieDialog_TypeSlider, this);
	mTypeSlider->SetValue((double)mSelectedType / (NUM_ZOMBIE_TYPES - 1));

	mRowSlider = new Slider(IMAGE_OPTIONS_SLIDERSLOT, IMAGE_OPTIONS_SLIDERTHUMB, SpawnZombieDialog_RowSlider, this);
	mRowSlider->SetValue(0);

	mColSlider = new Slider(IMAGE_OPTIONS_SLIDERSLOT, IMAGE_OPTIONS_SLIDERTHUMB, SpawnZombieDialog_ColSlider, this);
	mColSlider->SetValue(0);

	mHypnotizedCheckbox = MakeNewCheckbox(SpawnZombieDialog_Hypnotized, this, false);

	AddWidget(mTypeSlider);
	AddWidget(mRowSlider);
	AddWidget(mColSlider);
	AddWidget(mHypnotizedCheckbox);

	Resize(0, 0, 450, 480);
	
	mCloseButton->Resize(70, 400, 150, 42);
	mSpawnButton->Resize(230, 400, 150, 42);
	
	mTypeSlider->Resize(50, 110, 350, 30);
	mRowSlider->Resize(120, 160, 280, 30);
	mColSlider->Resize(120, 210, 280, 30);
	mHypnotizedCheckbox->Resize(50, 260, 46, 45);
}

SpawnZombieDialog::~SpawnZombieDialog()
{
	delete mTypeSlider;
	delete mRowSlider;
	delete mColSlider;
	delete mHypnotizedCheckbox;
}

void SpawnZombieDialog::Draw(Graphics* g)
{
	LawnDialog::Draw(g);
	
	g->SetFont(FONT_PICO129);
	g->SetColor(Color::White);
	
	std::string aTypeName = "Zom: " + std::string(gZombieDefs[mSelectedType].mZombieName);
	g->DrawString(aTypeName, 50, 100);
	
	g->DrawString("Row: " + std::to_string(mSelectedRow), 50, 182);
	g->DrawString("Col: " + std::to_string(mSelectedCol), 50, 232);
	g->DrawString("Hypnotized", 100, 290);
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
		mSelectedRow = (int)(theVal * 5 + 0.5); // 0-5
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
			Zombie* aZombie = mApp->mBoard->AddZombieInRow(mSelectedType, mSelectedRow, 0);
			if (aZombie)
			{
				aZombie->mX = (float)mApp->mBoard->GridToPixelX(mSelectedCol, mSelectedRow);
				if (mIsHypnotized) aZombie->StartMindControlled();
			}
		}
	}
}

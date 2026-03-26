#include "SpawnZombieDialog.h"
#include "../../LawnApp.h"
#include "../Board.h"
#include "../Zombie.h"
#include "../Plant.h"
#include "../../Resources.h"
#include "../LawnCommon.h"
#include "../../Sexy.TodLib/TodStringFile.h"
#include "GameButton.h"
#include "widget/Slider.h"
#include "widget/Checkbox.h"
#include "../../Sexy.TodLib/TodFoley.h"

using namespace Sexy;

SpawnZombieDialog::SpawnZombieDialog(LawnApp* theApp) :
	LawnDialog(theApp, Dialogs::DIALOG_SPAWN_ZOMBIE, true, "SPAWN ZOMBIES", "", "", Dialog::BUTTONS_NONE)
{
	mApp = theApp;
	mCurrentPage = 0;
	mSelectedType = ZOMBIE_NORMAL;
	mSelectedPlantType = SEED_PEASHOOTER;
	mSelectedRow = 0;
	mSelectedCol = 0;
	mIsHypnotized = false;
	mSpawnAllRows = false;
	mSpawnAllCols = false;

	mCloseButton = MakeButton(Dialog::ID_NO, this, "CLOSE");
	mSpawnButton = MakeButton(Dialog::ID_YES, this, "SPAWN!");

	mPrevButton = new Sexy::ButtonWidget(SpawnZombieDialog_PrevPage, this);
	mPrevButton->mFrameNoDraw = true;
	mNextButton = new Sexy::ButtonWidget(SpawnZombieDialog_NextPage, this);
	mNextButton->mFrameNoDraw = true;

	mTypeSlider = new Slider(IMAGE_OPTIONS_SLIDERSLOT, IMAGE_OPTIONS_SLIDERKNOB2, SpawnZombieDialog_TypeSlider, this);
	mTypeSlider->SetValue((double)mSelectedType / (NUM_ZOMBIE_TYPES - 1));

	mRowSlider = new Slider(IMAGE_OPTIONS_SLIDERSLOT, IMAGE_OPTIONS_SLIDERKNOB2, SpawnZombieDialog_RowSlider, this);
	mRowSlider->SetValue(0);

	mColSlider = new Slider(IMAGE_OPTIONS_SLIDERSLOT, IMAGE_OPTIONS_SLIDERKNOB2, SpawnZombieDialog_ColSlider, this);
	mColSlider->SetValue(0);

	mHypnotizedCheckbox = MakeNewCheckbox(SpawnZombieDialog_Hypnotized, this, false);
	mAllRowsCheckbox = MakeNewCheckbox(SpawnZombieDialog_AllRows, this, false);
	mAllColsCheckbox = MakeNewCheckbox(SpawnZombieDialog_AllCols, this, false);

	AddWidget(mCloseButton);
	AddWidget(mSpawnButton);
	AddWidget(mPrevButton);
	AddWidget(mNextButton);
	AddWidget(mTypeSlider);
	AddWidget(mRowSlider);
	AddWidget(mColSlider);
	AddWidget(mHypnotizedCheckbox);
	AddWidget(mAllRowsCheckbox);
	AddWidget(mAllColsCheckbox);

	CalcSize(300, 400);
}

SpawnZombieDialog::~SpawnZombieDialog()
{
	RemoveWidget(mCloseButton);
	RemoveWidget(mSpawnButton);
	RemoveWidget(mPrevButton);
	RemoveWidget(mNextButton);
	RemoveWidget(mTypeSlider);
	RemoveWidget(mRowSlider);
	RemoveWidget(mColSlider);
	RemoveWidget(mHypnotizedCheckbox);
	RemoveWidget(mAllRowsCheckbox);
	RemoveWidget(mAllColsCheckbox);
	
	delete mCloseButton;
	delete mSpawnButton;
	delete mPrevButton;
	delete mNextButton;
	delete mTypeSlider;
	delete mRowSlider;
	delete mColSlider;
	delete mHypnotizedCheckbox;
	delete mAllRowsCheckbox;
	delete mAllColsCheckbox;
}

void SpawnZombieDialog::Draw(Graphics* g)
{
	LawnDialog::Draw(g);
	
	g->SetFont(FONT_PICO129);
	g->SetColor(Color::White);
	
	if (mCurrentPage == 0)
	{
		std::string aTypeName = "Zombie: " + TodStringTranslate(StrFormat("[%s]", gZombieDefs[mSelectedType].mZombieName));
		g->DrawString(aTypeName, 50, 110);
		g->DrawString("Row: " + std::to_string(mSelectedRow), 50, 222);
		g->DrawString("Col: " + std::to_string(mSelectedCol), 50, 282);
		g->DrawString("Hypnotize (Ally)", 110, 352);
	}
	else
	{
		PlantDefinition& aPlantDef = GetPlantDefinition(mSelectedPlantType);
		std::string aTypeName = "Plant: " + TodStringTranslate(StrFormat("[%s]", aPlantDef.mPlantName));
		g->DrawString(aTypeName, 50, 110);
		g->DrawString("Row: " + std::to_string(mSelectedRow), 50, 222);
		g->DrawString("Col: " + std::to_string(mSelectedCol), 50, 282);
		g->DrawString("Awaken (Coffee)", 110, 352);
	}

	g->DrawString("All Rows", 110, 402);
	g->DrawString("All Columns", 310, 402);

	if (mApp->mBoard)
	{
		std::string aRowLimit = mApp->mBoard->StageHas6Rows() ? "(0-5)" : "(0-4)";
		g->DrawString(aRowLimit, 50, 238);
	}

	int aPrevOffsetY = mPrevButton->mIsDown ? 1 : 0;
	if (IMAGE_ZEN_NEXTGARDEN && mCurrentPage > 0)
	{
		g->DrawImageMirror(IMAGE_ZEN_NEXTGARDEN, mPrevButton->mX, mPrevButton->mY + aPrevOffsetY, true);
	}

	int aNextOffsetY = mNextButton->mIsDown ? 1 : 0;
	if (IMAGE_ZEN_NEXTGARDEN && mCurrentPage < 1)
	{
		g->DrawImage(IMAGE_ZEN_NEXTGARDEN, mNextButton->mX, mNextButton->mY + aNextOffsetY);
	}
}

void SpawnZombieDialog::Update()
{
	LawnDialog::Update();

	int aButtonW = 210;
	int aButtonH = 46;
	mCloseButton->Resize(mWidth / 2 - aButtonW - 10, mHeight - 65, aButtonW, aButtonH);
	mSpawnButton->Resize(mWidth / 2 + 10, mHeight - 65, aButtonW, aButtonH);

	int aArrowWidth = IMAGE_ZEN_NEXTGARDEN ? IMAGE_ZEN_NEXTGARDEN->mWidth : 45;
	int aMargin = 20;

	mPrevButton->Resize(10, mHeight - 80, aArrowWidth, 40);
	mNextButton->Resize(mWidth - aMargin - aArrowWidth, mHeight - 80, aArrowWidth, 40);
	mPrevButton->SetVisible(mCurrentPage > 0);
	mNextButton->SetVisible(mCurrentPage < 1);

	mTypeSlider->Resize(50, 120, mWidth - 100, 30);
	mRowSlider->Resize(150, 200, mWidth - 200, 30);
	mColSlider->Resize(150, 260, mWidth - 200, 30);
	
	mHypnotizedCheckbox->Resize(50, 320, 46, 45);
	mAllRowsCheckbox->Resize(50, 370, 46, 45);
	mAllColsCheckbox->Resize(250, 370, 46, 45);
}

void SpawnZombieDialog::SliderVal(int theId, double theVal)
{
	if (theId == SpawnZombieDialog_TypeSlider)
	{
		if (mCurrentPage == 0)
		{
			mSelectedType = (ZombieType)(int)(theVal * (NUM_ZOMBIE_TYPES - 1) + 0.5);
		}
		else
		{
			mSelectedPlantType = (SeedType)(int)(theVal * (NUM_SEED_TYPES - 1) + 0.5);
		}
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
	else if (theId == SpawnZombieDialog_AllRows)
	{
		mSpawnAllRows = checked;
	}
	else if (theId == SpawnZombieDialog_AllCols)
	{
		mSpawnAllCols = checked;
	}
}

void SpawnZombieDialog::ButtonDepress(int theId)
{
	if (theId == Dialog::ID_NO) // Close button
	{
		mApp->KillSpawnZombieDialog();
	}
	else if (theId == Dialog::ID_YES) // Spawn button
	{
		mApp->PlaySample(SOUND_BUTTONCLICK);
		if (mApp->mBoard)
		{
			// Verify row is valid for current stage
			int aMaxRow = mApp->mBoard->StageHas6Rows() ? 5 : 4;
			if (mSelectedRow > aMaxRow) mSelectedRow = aMaxRow;

			int startRow = mSpawnAllRows ? 0 : mSelectedRow;
			int endRow = mSpawnAllRows ? aMaxRow : mSelectedRow;

			int startCol = mSpawnAllCols ? 0 : mSelectedCol;
			int endCol = mSpawnAllCols ? 8 : mSelectedCol;

			for (int r = startRow; r <= endRow; r++)
			{
				for (int c = startCol; c <= endCol; c++)
				{
					if (mCurrentPage == 0)
					{
						Zombie* aZombie = mApp->mBoard->AddZombieInRow(mSelectedType, r, 0);
						if (aZombie)
						{
							float aX = (float)mApp->mBoard->GridToPixelX(c, r);
							aZombie->mPosX = aX;
							aZombie->mX = aX;
							
							// Fix Y position for roof/slopes
							aZombie->mPosY = aZombie->GetPosYBasedOnRow(r);
							aZombie->mY = aZombie->mPosY;

							if (mIsHypnotized) aZombie->StartMindControlled();
						}
					}
					else
					{
						Plant* aPlant = mApp->mBoard->AddPlant(c, r, mSelectedPlantType);
						if (aPlant && mIsHypnotized)
						{
							aPlant->mIsAsleep = false;
						}
					}
				}
			}
		}
	}
	else if (theId == SpawnZombieDialog_PrevPage && mCurrentPage > 0)
	{
		mApp->PlaySample(SOUND_TAP);
		mCurrentPage--;
		mDialogHeader = mCurrentPage == 0 ? "SPAWN ZOMBIES" : "SPAWN PLANTS";
		mTypeSlider->SetValue(mCurrentPage == 0 ? ((double)mSelectedType / (NUM_ZOMBIE_TYPES - 1)) : ((double)mSelectedPlantType / (NUM_SEED_TYPES - 1)));
	}
	else if (theId == SpawnZombieDialog_NextPage && mCurrentPage < 1)
	{
		mApp->PlaySample(SOUND_TAP);
		mCurrentPage++;
		mDialogHeader = mCurrentPage == 0 ? "SPAWN ZOMBIES" : "SPAWN PLANTS";
		mTypeSlider->SetValue(mCurrentPage == 0 ? ((double)mSelectedType / (NUM_ZOMBIE_TYPES - 1)) : ((double)mSelectedPlantType / (NUM_SEED_TYPES - 1)));
	}
}

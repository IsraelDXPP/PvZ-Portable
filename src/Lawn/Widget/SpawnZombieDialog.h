#ifndef __SPAWNZOMBIEDIALOG_H__
#define __SPAWNZOMBIEDIALOG_H__

#include "LawnDialog.h"
#include "widget/SliderListener.h"
#include "widget/CheckboxListener.h"

class LawnApp;
class GameButton;
namespace Sexy { class Checkbox; class Slider; }

class SpawnZombieDialog : public LawnDialog, public Sexy::SliderListener, public Sexy::CheckboxListener
{
public:
	enum
	{
		SpawnZombieDialog_TypeSlider,
		SpawnZombieDialog_RowSlider,
		SpawnZombieDialog_ColSlider,
		SpawnZombieDialog_Hypnotized,
		SpawnZombieDialog_AllRows,
		SpawnZombieDialog_AllCols,
		SpawnZombieDialog_PrevPage,
		SpawnZombieDialog_NextPage
	};

	LawnApp*			mApp;
	Sexy::ButtonWidget*	mPrevButton;
	Sexy::ButtonWidget*	mNextButton;
	Sexy::Slider*		mTypeSlider;
	Sexy::Slider*		mRowSlider;
	Sexy::Slider*		mColSlider;
	Sexy::Checkbox*		mHypnotizedCheckbox;
	Sexy::Checkbox*		mAllRowsCheckbox;
	Sexy::Checkbox*		mAllColsCheckbox;

	int					mCurrentPage;
	ZombieType			mSelectedType;
	SeedType			mSelectedPlantType;
	int					mSelectedRow;
	int					mSelectedCol;
	bool				mIsHypnotized;
	bool				mSpawnAllRows;
	bool				mSpawnAllCols;

public:
	SpawnZombieDialog(LawnApp* theApp);
	virtual ~SpawnZombieDialog();

	virtual void		Draw(Graphics* g);
	virtual void		Update();
	virtual void		ButtonDepress(int theId);
	virtual void		SliderVal(int theId, double theVal);
	virtual void		CheckboxChecked(int theId, bool checked);
};

#endif

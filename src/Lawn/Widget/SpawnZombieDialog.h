#ifndef __SPAWNZOMBIEDIALOG_H__
#define __SPAWNZOMBIEDIALOG_H__

#include "LawnDialog.h"
#include "widget/Slider.h"
#include "widget/Checkbox.h"

class LawnApp;
class GameButton;

class SpawnZombieDialog : public LawnDialog, public SliderListener, public CheckboxListener
{
public:
	enum
	{
		SpawnZombieDialog_Close,
		SpawnZombieDialog_Spawn,
		SpawnZombieDialog_TypeSlider,
		SpawnZombieDialog_RowSlider,
		SpawnZombieDialog_ColSlider,
		SpawnZombieDialog_Hypnotized
	};

	LawnApp*			mApp;
	GameButton*			mCloseButton;
	GameButton*			mSpawnButton;
	Slider*				mTypeSlider;
	Slider*				mRowSlider;
	Slider*				mColSlider;
	Checkbox*			mHypnotizedCheckbox;

	ZombieType			mSelectedType;
	int					mSelectedRow;
	int					mSelectedCol;
	bool				mIsHypnotized;

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

/*
 * Portions of this file are based on the PopCap Games Framework
 * Copyright (C) 2005-2009 PopCap Games, Inc.
 * 
 * Copyright (C) 2026 Zhou Qiankang <wszqkzqk@qq.com>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later AND LicenseRef-PopCap
 *
 * This file is part of PvZ-Portable.
 *
 * PvZ-Portable is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PvZ-Portable is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with PvZ-Portable. If not, see <https://www.gnu.org/licenses/>.
 */

#include <switch.h>
#include <unordered_map>

#include "SexyAppBase.h"
#include "graphics/GLInterface.h"
#include "graphics/GLImage.h"
#include "widget/WidgetManager.h"

using namespace Sexy;

static PadState pad;
static std::unordered_map<u64, KeyCode> keyMaps = {
	{HidNpadButton_Plus,  KEYCODE_ESCAPE},
	{HidNpadButton_Minus, KEYCODE_SPACE},
	{HidNpadButton_A,     KEYCODE_RETURN},
	{HidNpadButton_B,     KEYCODE_ESCAPE},
	{HidNpadButton_X,     KEYCODE_SPACE},
	{HidNpadButton_L,     KEYCODE_LBUTTON},
	{HidNpadButton_R,     KEYCODE_RBUTTON},
};

void SexyAppBase::InitInput()
{
	// Configure our supported input layout: a single player with standard controller styles
	padConfigureInput(1, HidNpadStyleSet_NpadStandard);

	// Initialize the default gamepad (which reads handheld mode inputs as well as the first connected controller)
	padInitializeDefault(&pad);

	hidInitializeTouchScreen();

	if (!mMouseIn)
		mMouseIn = true;
}

bool SexyAppBase::StartTextInput(std::string& theInput)
{
	char buf[512];

	SwkbdConfig kbd;
	swkbdCreate(&kbd, 0);
	swkbdConfigMakePresetDefault(&kbd);
	swkbdConfigSetType(&kbd, SwkbdType_Normal);

	swkbdConfigSetGuideText(&kbd, "Enter text...");
	swkbdConfigSetOkButtonText(&kbd, "OK");

	Result rc = swkbdShow(&kbd, buf, sizeof(buf));
	swkbdClose(&kbd);

	if (R_SUCCEEDED(rc))
	{
		theInput = buf;
		return true;
	}

	return false;
}

void SexyAppBase::StopTextInput()
{
	
}

bool SexyAppBase::ProcessDeferredMessages(bool singleMessage)
{
	if (!appletMainLoop())
	{
		mShutdown = true;
		return false;
	}

	static s32 prev_touchcount = 0;

	// Scan the gamepad. This should be done once for each frame
	padUpdate(&pad);

	u64 kDown = padGetButtonsDown(&pad);
	u64 kUp = padGetButtonsUp(&pad);
	u64 kHeld = padGetButtonsHeld(&pad);

	static int x=1280/2, y=720/2;
	bool mouseMoved = false;

	// Analog sticks for cursor movement
	HidAnalogStickState lStick = padGetStickPos(&pad, 0);
	if (abs(lStick.x) > 5000 || abs(lStick.y) > 5000)
	{
		mLastUserInputTick = mLastTimerTime;
		x += lStick.x / 10000;
		y -= lStick.y / 10000; // Y is inverted on sticks vs screen

		if (x < 0) x = 0;
		if (x > 1280) x = 1280;
		if (y < 0) y = 0;
		if (y > 720) y = 720;
		mouseMoved = true;
	}

	if (kDown)
	{
		mLastUserInputTick = mLastTimerTime;

		for (auto& k : keyMaps)
		{
			if (kDown & k.first)
			{
				mWidgetManager->KeyDown(k.second);
				if (k.first == HidNpadButton_A)
					mWidgetManager->MouseDown(x, y, 1);
			}
		}
	}

	if (kUp)
	{
		mLastUserInputTick = mLastTimerTime;

		for (auto& k : keyMaps)
		{
			if (kUp & k.first)
			{
				mWidgetManager->KeyUp(k.second);
				if (k.first == HidNpadButton_A)
					mWidgetManager->MouseUp(x, y, 1);
			}
		}
	}

	HidTouchScreenState state = {0};
	hidGetTouchScreenStates(&state, 1);
	if (state.count)
	{
		mLastUserInputTick = mLastTimerTime;

		int nx = (int)state.touches[0].x;
		int ny = (int)state.touches[0].y;
		
		if (nx != x || ny != y)
		{
			x = nx;
			y = ny;
			mouseMoved = true;
			if (prev_touchcount)
				mWidgetManager->MouseDrag(x, y);
		}
	}
	else if (mouseMoved && kHeld & HidNpadButton_A)
	{
		mWidgetManager->MouseDrag(x, y);
	}

	if (mouseMoved)
	{
		mWidgetManager->RemapMouse(x, y);
		if (!prev_touchcount)
			mWidgetManager->MouseMove(x, y);
	}

	if (state.count && !prev_touchcount)
		mWidgetManager->MouseDown(x, y, 1);
	else if (!state.count && prev_touchcount)
		mWidgetManager->MouseUp(x, y, 1);

	prev_touchcount = state.count;

	return false;
}

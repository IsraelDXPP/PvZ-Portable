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

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <switch.h>

#include "SexyAppBase.h"
#include "graphics/GLInterface.h"
#include "graphics/GLImage.h"
#include "widget/WidgetManager.h"

using namespace Sexy;

// -------------------------------
// Variable global para depuración con GDB
volatile int wait_for_gdb = 1;
// -------------------------------

void SexyAppBase::MakeWindow()
{
    // Espera hasta que GDB cambie wait_for_gdb a 0
    while(wait_for_gdb);

    // Connect to the EGL default display
    mWindow = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (!mWindow)
    {
        printf("MakeWindow: eglGetDisplay failed with error 0x%04X\n", eglGetError());
        return;
    }

    if (eglInitialize(mWindow, nullptr, nullptr) == EGL_FALSE)
    {
        printf("MakeWindow: eglInitialize failed with error 0x%04X\n", eglGetError());
        return;
    }

    if (eglBindAPI(EGL_OPENGL_ES_API) == EGL_FALSE)
    {
        printf("MakeWindow: eglBindAPI failed with error 0x%04X\n", eglGetError());
        eglTerminate(mWindow);
        return;
    }

    EGLConfig config;
    EGLint numConfigs;
    static const EGLint framebufferAttributeList[] =
    {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
        EGL_RED_SIZE,        8,
        EGL_GREEN_SIZE,      8,
        EGL_BLUE_SIZE,       8,
        EGL_ALPHA_SIZE,      8,
        EGL_DEPTH_SIZE,      24,
        EGL_STENCIL_SIZE,    8,
        EGL_NONE
    };

    if (eglChooseConfig(mWindow, framebufferAttributeList, &config, 1, &numConfigs) == EGL_FALSE || numConfigs == 0)
    {
        printf("MakeWindow: eglChooseConfig failed with error 0x%04X, numConfigs=%d\n", eglGetError(), numConfigs);
        eglTerminate(mWindow);
        return;
    }

    NWindow* nativeWin = nwindowGetDefault();
    nwindowSetDimensions(nativeWin, 1920, 1080); // Optional: ensure the NWindow is sized
    
    mSurface = eglCreateWindowSurface(mWindow, config, nativeWin, nullptr);
    if (!mSurface)
    {
        printf("MakeWindow: eglCreateWindowSurface failed with error 0x%04X\n", eglGetError());
        eglTerminate(mWindow);
        return;
    }

    static const EGLint contextAttributeList[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    mContext = eglCreateContext(mWindow, config, EGL_NO_CONTEXT, contextAttributeList);
    if (!mContext)
    {
        printf("MakeWindow: eglCreateContext failed with error 0x%04X\n", eglGetError());
        eglDestroySurface(mWindow, mSurface);
        eglTerminate(mWindow);
        return;
    }

    if (eglMakeCurrent(mWindow, mSurface, mSurface, mContext) == EGL_FALSE)
    {
        printf("MakeWindow: eglMakeCurrent failed with error 0x%04X\n", eglGetError());
        eglDestroyContext(mWindow, mContext);
        eglDestroySurface(mWindow, mSurface);
        eglTerminate(mWindow);
        return;
    }

    // Verify context loaded
    if (!glCreateShader) 
    {
        printf("MakeWindow: glCreateShader is NULL! GLES2 driver not linked properly.\n");
    }

    eglSwapInterval(mWindow, 1);

    if (mGLInterface == nullptr)
    {
        mGLInterface = new GLInterface(this);
        if (!InitGLInterface())
        {
            delete mGLInterface;
            mGLInterface = nullptr;
            return;
        }

        mGLInterface->UpdateViewport();
        mWidgetManager->Resize(mScreenBounds, mGLInterface->mInputSourceRect);
    }

    bool isActive = mActive;
    mActive = true;
    mPhysMinimized = false;

    if (isActive != mActive)
        RehupFocus();

    ReInitImages();

    mWidgetManager->mImage = mGLInterface->GetScreenImage();
    mWidgetManager->MarkAllDirty();
}
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

#include <EGL/egl.h>    // EGL library
#include <EGL/eglext.h> // EGL extensions

#include <switch.h>

#include "SexyAppBase.h"
#include "graphics/GLInterface.h"
#include "graphics/GLImage.h"
#include "widget/WidgetManager.h"

using namespace Sexy;

#include <stdio.h>

extern "C" void __libnx_exception_handler(ThreadExceptionDump *ctx)
{
	FILE* f = fopen("sdmc:/pvz_error_log.txt", "w");
	if (f)
	{
		fprintf(f, "CRASH DETECTED!\n");
		if (ctx) {
			fprintf(f, "Error Desc: %x\n", ctx->error_desc);
			fprintf(f, "pc:  %016llx\n", (unsigned long long)ctx->pc.x);
			fprintf(f, "lr:  %016llx\n", (unsigned long long)ctx->lr.x);
			fprintf(f, "sp:  %016llx\n", (unsigned long long)ctx->sp.x);
			fprintf(f, "fp:  %016llx\n", (unsigned long long)ctx->fp.x);
		}
		fclose(f);
	}
}

void SexyAppBase::MakeWindow()
{

    // Connect to the EGL default display
    mWindow = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (!mWindow)
		return;

	// Initialize the EGL display connection
	if (eglInitialize(mWindow, nullptr, nullptr) == EGL_FALSE)
	{
		eglTerminate(mWindow);
		return;
	}

	// Select OpenGL (Core) as the desired graphics API
	if (eglBindAPI(EGL_OPENGL_API) == EGL_FALSE)
	{
		eglTerminate(mWindow);
		return;
	}

	// Get an appropriate EGL framebuffer configuration
	EGLConfig config;
	EGLint numConfigs;
	static const EGLint framebufferAttributeList[] =
	{
		EGL_RED_SIZE,     8,
		EGL_GREEN_SIZE,   8,
		EGL_BLUE_SIZE,    8,
		EGL_ALPHA_SIZE,   8,
		EGL_DEPTH_SIZE,   24,
		EGL_STENCIL_SIZE, 8,
		EGL_NONE
	};
	if (eglChooseConfig(mWindow, framebufferAttributeList, &config, 1, &numConfigs) == EGL_FALSE)
	{
		eglTerminate(mWindow);
		return;
	}
	if (numConfigs == 0)
	{
		eglTerminate(mWindow);
		return;
	}

	// Create an EGL window surface
	mSurface = eglCreateWindowSurface(mWindow, config, nwindowGetDefault(), nullptr);
	if (!mSurface)
	{
		eglTerminate(mWindow);
		return;
	}

	// Create an EGL rendering context (OpenGL 4.3 Core Profile).
	static const EGLint contextAttributeList[] =
	{
		EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR,
		EGL_CONTEXT_MAJOR_VERSION_KHR, 4,
		EGL_CONTEXT_MINOR_VERSION_KHR, 3,
		EGL_NONE
	};
	mContext = eglCreateContext(mWindow, config, EGL_NO_CONTEXT, contextAttributeList);
	if (!mContext)
	{
		eglDestroySurface(mWindow, mSurface);
		eglTerminate(mWindow);
		return;
	}

	// Connect the context to the surface
	if (eglMakeCurrent(mWindow, mSurface, mSurface, mContext) == EGL_FALSE)
	{
		eglDestroyContext(mWindow, mContext);
		mContext = nullptr;
		eglDestroySurface(mWindow, mSurface);
		mSurface = nullptr;
		eglTerminate(mWindow);
		mWindow = nullptr;
		return;
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
		mWidgetManager->Resize(mScreenBounds, mGLInterface->mPresentationRect);
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
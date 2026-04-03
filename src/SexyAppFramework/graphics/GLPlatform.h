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

#ifndef __GLPLATFORM_H__
#define __GLPLATFORM_H__

/*
 * Platform-specific GL includes
 *
 * We explicitly rely on __SWITCH__ (defined by devkitPro/libnx toolchains).
 * Do NOT use custom macros like NINTENDO_SWITCH, as they may not be defined
 * in all build environments (e.g. CI, different toolchains).
 */
#ifdef __SWITCH__

// Switch uses libnx + EGL + GLES2 directly.
// All symbols are provided by the SDK, no loader (like glad) is required.
#include <switch.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#else

/*
 * Desktop / non-Switch platforms
 *
 * We rely on glad to dynamically load GLES2 symbols.
 * SDL is used as the backend for retrieving function pointers.
 */
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4551) // glad-generated code triggers this on MSVC
#endif

#include <glad/gles2.h>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <SDL.h>

#endif // __SWITCH__

/*
 * GLSL compatibility layer
 *
 * The original PopCap codebase mixes GLSL 1.20 (desktop GL) and
 * GLSL ES 1.00 (OpenGL ES). These macros normalize shader keywords
 * so the same shader sources can compile on both targets.
 *
 * IMPORTANT:
 * These MUST be available on all platforms (including Switch),
 * since shader compilation is shared code.
 */
#define GLSL_VERT_MACROS \
	"#define VERT_IN attribute\n" \
	"#define V2F varying\n"

#define GLSL_FRAG_MACROS \
	"#define V2F varying\n" \
	"#define FRAG_OUT gl_FragColor\n" \
	"#define TEX2D texture2D\n"

/*
 * Indicates whether a desktop GL fallback context is being used.
 * This affects shader generation paths elsewhere in the renderer.
 */
extern bool gDesktopGLFallback;

/*
 * Platform-specific GL initialization
 *
 * On desktop:
 *   - We must manually load GLES2 function pointers via glad.
 *
 * On Switch:
 *   - Function pointers are already resolved by libnx/EGL.
 *   - Calling gladLoadGLES2 here would either fail or cause crashes.
 *   - Therefore, this is intentionally a no-op on Switch.
 */
inline void PlatformGLInit()
{
#ifndef __SWITCH__
	// Load GL function pointers using SDL as the provider
	gladLoadGLES2((GLADloadfunc)SDL_GL_GetProcAddress);
#else
	// No initialization required on Switch (handled by EGL/libnx)
#endif
}

#endif // __GLPLATFORM_H__

#ifndef __GLPLATFORM_H__
#define __GLPLATFORM_H__

// =======================
// Nintendo Switch
// =======================
#ifdef __SWITCH__

#include <switch.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

// En Switch NO usamos glad
inline void PlatformGLInit()
{
    // Nada que hacer, EGL ya expone las funciones
}

// =======================
// Desktop / Otros
// =======================
#else

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4551)
#endif

#include <glad/gles2.h>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <SDL.h>

// Inicialización GL para desktop
inline void PlatformGLInit()
{
    gladLoadGLES2((GLADloadfunc)SDL_GL_GetProcAddress);
}

#endif

// =======================
// GLSL macros (compartidas)
// =======================

#define GLSL_VERT_MACROS \
    "#define VERT_IN attribute\n" \
    "#define V2F varying\n"

#define GLSL_FRAG_MACROS \
    "#define V2F varying\n" \
    "#define FRAG_OUT gl_FragColor\n" \
    "#define TEX2D texture2D\n"

// =======================

extern bool gDesktopGLFallback;

#endif // __GLPLATFORM_H__

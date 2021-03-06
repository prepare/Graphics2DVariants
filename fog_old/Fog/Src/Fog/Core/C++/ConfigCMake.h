// [Fog-Core]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_CORE_CPP_CONFIGCMAKE_H
#define _FOG_CORE_CPP_CONFIGCMAKE_H

// ============================================================================
// [FOG_BUILD]
// ============================================================================

//! @brief Whether to build Fog-Framework as a static library.
/* #undef FOG_BUILD_STATIC */

//! @brief Whether to build Fog/UI.
/* #undef FOG_BUILD_UI */

//! @brief Whether to build Fog/UI-X11 support.
/* #undef FOG_BUILD_UI_X11 */

//! @brief Whether to build Fog/UI-X11 module.
/* #undef FOG_BUILD_UI_X11_MODULE */

// ============================================================================
// [FOG_DEBUG]
// ============================================================================

// If FOG_DEBUG and FOG_NO_DEBUG is not defined then FOG_DEBUG will be detected
// using the compiler specific macros. This enables to set the build type using
// IDE.
#if !defined(FOG_DEBUG) && !defined(FOG_NO_DEBUG)

#if defined(_DEBUG)
#define FOG_DEBUG
#endif // _DEBUG

#endif // !FOG_DEBUG && !FOG_NO_DEBUG

// ============================================================================
// [FOG_BYTE_ORDER]
// ============================================================================

//! @brief Target byteorder, can be:
//!   - @c FOG_BIG_ENDIAN    == 0.
//!   - @c FOG_LITTLE_ENDIAN == 1,
#define FOG_BYTE_ORDER 1

// ============================================================================
// [FOG_OPTIMIZE]
// ============================================================================

//! @brief Enable support for x86/x64 MMX instructions.
//!
//! @note MMX is not widely used today, because of SSE2/AVX.
#define FOG_OPTIMIZE_MMX

//! @brief Enable support for x86/x64 3dNow! instructions.
//!
//! @note 3dNow! is only implemented by AMD and at this time it's deprecated.
/* #undef FOG_OPTIMIZE_3DNOW */

//! @brief Enable support for x86/x64 SSE instructions.
#define FOG_OPTIMIZE_SSE

//! @brief Enable support for x86/x64 SSE2 instructions.
#define FOG_OPTIMIZE_SSE2

//! @brief Enable support for x86/x64 SSSE3 instructions.
#define FOG_OPTIMIZE_SSSE3

//! @brief Enable support for ARM Neon instructions.
/* #undef FOG_OPTIMIZE_NEON */

// ============================================================================
// [Header Files]
// ============================================================================

//! @brief Defined if we have fenv.h
/* #undef FOG_HAVE_FENV_H */
//! @brief Defined if we have float.h
#define FOG_HAVE_FLOAT_H
//! @brief Defined if we have limits.h
#define FOG_HAVE_LIMITS_H
//! @brief Defined if we have stdarg.h
/* #undef FOG_HAVE_STDARG_H */
//! @brief Defined if we have stdint.h
/* #undef FOG_HAVE_STDINT_H */
//! @brief Defined if we have unistd.h
/* #undef FOG_HAVE_UNISTD_H */

// ============================================================================
// [Libraries]
// ============================================================================

//! @brief Defined if jpeg library is available.
/* #undef FOG_HAVE_LIBJPEG */
//! @brief Defined if png library is available.
/* #undef FOG_HAVE_LIBPNG */

// ============================================================================
// [Fog-Font Backends]
// ============================================================================

//! @brief Whether to build Windows font support.
//!
//! This is default when using Windows.
#define FOG_FONT_WINDOWS

//! @brief Whether to build Mac font support.
//!
//! This is default when using Mac.
/* #undef FOG_FONT_MAC */

//! @brief Whether to build FontConfig support.
//!
//! Can be enabled only if FOG_FONT_FREETYPE is defined.
/* #undef FOG_HAVE_FONTCONFIG */

//! @brief Whether to build FreeType support
//!
//! This is default when using Linux/BSD. May be used together with Windows/Mac
//! font support.
/* #undef FOG_FONT_FREETYPE */

// ============================================================================
// [FOG_INSTALL]
// ============================================================================

//! @brief Install prefix specified by configure parameters.
#define FOG_INSTALL_PREFIX "C:/Program Files/Fog"

// [Guard]
#endif // _FOG_CORE_CPP_CONFIGCMAKE_H

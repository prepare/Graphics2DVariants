
/*******************************************************************\

This file is part of the AggOO library.  AggOO provides a way to emulate
GDI+ using Anti-Grain Geometry (http://www.antigrain.com) as the rendering
backend.

AggOO - Version 0.1
Copyright (c) 2006 Dratek Software

Anti-Grain Geometry - Version 2.4 
Copyright (C) 2002-2005 Maxim Shemanarev (McSeem) 

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions 
are met:

  1. Redistributions of source code must retain the above copyright 
     notice, this list of conditions and the following disclaimer. 

  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in 
     the documentation and/or other materials provided with the 
     distribution. 

  3. The name of the author may not be used to endorse or promote 
     products derived from this software without specific prior 
     written permission. 

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR 
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.

\*******************************************************************/

/////////////////////////////////////////////////////////////////////
// $Id: AggOOPlatform.h,v 1.4 2010/03/12 04:20:57 dratek Exp $
//
// AggOOPlatform.h -- Sets some flags based on the target platform.
//
// Author       : Chad M. Draper
// Date         : September 6, 2006
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2006-09-06      Initial implementation
//

#ifndef AggOOPlatform_H_
#define AggOOPlatform_H_

//#include <vld.h>
//#define _CRTDBG_MAP_ALLOC

#include "AggOOConfig.h"

namespace AggOO
{
    // Platform and compiler flags.  This is lifted from the Ogre library,
    // with the names changed to fit the AggOO library.
    #define AGGOO_PLATFORM_WIN32 1
    #define AGGOO_PLATFORM_LINUX 2
    #define AGGOO_PLATFORM_APPLE 3

    #define AGGOO_COMPILER_MSVC 1
    #define AGGOO_COMPILER_GNUC 2
    #define AGGOO_COMPILER_BORL 3

    #define AGGOO_ENDIAN_LITTLE 1
    #define AGGOO_ENDIAN_BIG 2

    #define AGGOO_ARCHITECTURE_32 1
    #define AGGOO_ARCHITECTURE_64 2

    // Set the flags to indicate the current compiler and version:
    #if defined( _MSC_VER )
    #   define AGGOO_COMPILER AGGOO_COMPILER_MSVC
    #   define AGGOO_COMP_VER _MSC_VER

    #elif defined( __GNUC__ )
    #   define AGGOO_COMPILER AGGOO_COMPILER_GNUC
    #   define AGGOO_COMP_VER (((__GNUC__)*100) + \
            (__GNUC_MINOR__*10) + \
            __GNUC_PATCHLEVEL__)

    #elif defined( __BORLANDC__ )
    #   define AGGOO_COMPILER AGGOO_COMPILER_BORL
    #   define AGGOO_COMP_VER __BCPLUSPLUS__

    #else
    #   pragma error "ERROR: Compiler unknown.  Abort."

    #endif

    /* See if we can use __forceinline or if we need to use __inline instead */
    #if AGGOO_COMPILER == AGGOO_COMPILER_MSVC
    #   if AGGOO_COMP_VER >= 1200
    #       define FORCEINLINE __forceinline
    #   endif
    #else
    #   define FORCEINLINE __inline
    #endif

    // Set flags to indicate the current platform
    #if defined( __WIN32__ ) || defined( _WIN32 )
    #   define AGGOO_PLATFORM AGGOO_PLATFORM_WIN32

    #elif defined( __APPLE_CC__)
    #   define AGGOO_PLATFORM AGGOO_PLATFORM_APPLE

    #else
    #   define AGGOO_PLATFORM AGGOO_PLATFORM_LINUX
    #endif

    #if (_WIN32_WCE < 0x500) && ( defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP) )
        #pragma comment(lib, "ccrtrtti.lib")
        #ifdef _X86_
            #if defined(_DEBUG)
                #pragma comment(lib, "libcmtx86d.lib")
            #else
                #pragma comment(lib, "libcmtx86.lib")
            #endif
        #endif
    #endif 

    // Architecture type (32 to 64 bit, currently.)
    /* Find the arch type */
    #if defined(__x86_64__)
    #   define AGGOO_ARCH_TYPE AGGOO_ARCHITECTURE_64
    #else
    #   define AGGOO_ARCH_TYPE AGGOO_ARCHITECTURE_32
    #endif

    // For generating compiler warnings - should work on any compiler
    // As a side note, if you start your message with 'Warning: ', the MSVC
    // IDE actually does catch a warning :)
    #define AGGOO_QUOTE_INPLACE(x) # x
    #define AGGOO_QUOTE(x) AGGOO_QUOTE_INPLACE(x)
    #define AGGOO_WARN( x )  message( __FILE__ "(" QUOTE( __LINE__ ) ") : " x "\n" )

    //----------------------------------------------------------------------------
    // Settings for Windows systems
    #if AGGOO_PLATFORM == AGGOO_PLATFORM_WIN32

    // If we're not including this from a client build, specify that the stuff
    // should get exported. Otherwise, import it.
    #   if defined( __MINGW32__ )
            // Linux compilers don't have symbol import/export directives.
    #       define _AggOOExport
    #       define _AggOOPrivate
    #   else
    #       if AGGOO_DYNAMIC_LINKAGE == 1
    #           if defined( AGGOO_NONCLIENT_BUILD )
    #               define _AggOOExport __declspec( dllexport )
    #           else
    #               define _AggOOExport __declspec( dllimport )
    #           endif
    #       else
    #           define _AggOOExport
    #       endif
    #       define _AggOOPrivate
    #   endif
    // Win32 compilers use _DEBUG for specifying debug builds.
    #   ifdef _DEBUG
    #       define AGGOO_DEBUG_MODE 1
    #   else
    #       define AGGOO_DEBUG_MODE 0
    #   endif

/*
    #if defined( __MINGW32__ )
        #define EXT_HASH
    #else
        #define snprintf _snprintf
        #define vsnprintf _vsnprintf
    #endif
*/

/*
    #if AGGOO_DEBUG_MODE
        #define AGGOO_PLATFORM_LIB "AggOOPlatform_d.dll"
    #else
        #define AGGOO_PLATFORM_LIB "AggOOPlatform.dll"
    #endif
*/

    #endif
    //----------------------------------------------------------------------------

    //----------------------------------------------------------------------------
    // Linux/Apple Settings
    #if AGGOO_PLATFORM == AGGOO_PLATFORM_LINUX || AGGOO_PLATFORM == AGGOO_PLATFORM_APPLE

    // Enable GCC 4.0 symbol visibility
    #   if AGGOO_COMP_VER >= 400
    #       define _AggOOExport  __attribute__ ((visibility("default")))
    #       define _AggOOPrivate __attribute__ ((visibility("hidden")))
    #   else
    #       define _AggOOExport
    #       define _AggOOPrivate
    #   endif

    // A quick define to overcome different names for the same function
    #   define stricmp strcasecmp

    // Unlike the Win32 compilers, Linux compilers seem to use DEBUG for when
    // specifying a debug build.
    #   ifdef DEBUG
    #       define AGGOO_DEBUG_MODE 1
    #   else
    #       define AGGOO_DEBUG_MODE 0
    #   endif

    #if AGGOO_PLATFORM == AGGOO_PLATFORM_APPLE
        #define AGGOO_PLATFORM_LIB "AggOOPlatform.bundle"
    #else
        //AGGOO_PLATFORM_LINUX
        #define AGGOO_PLATFORM_LIB "libAggOOPlatform.so"
    #endif

    #endif

    //For apple, we always have a custom config.h file
    #if AGGOO_PLATFORM == AGGOO_PLATFORM_APPLE
    #    include "config.h"
/*
    //SDL_main must be included in the file that contains
    //the application's main() function.
    #ifndef AGGOO_NONCLIENT_BUILD
    #   include <SDL/SDL_main.h>
    #endif
*/

    #endif

    //----------------------------------------------------------------------------

    //----------------------------------------------------------------------------
    // Endian Settings
    // check for BIG_ENDIAN config flag, set AGGOO_ENDIAN correctly
    #ifdef CONFIG_BIG_ENDIAN
    #    define AGGOO_ENDIAN AGGOO_ENDIAN_BIG
    #else
    #    define AGGOO_ENDIAN AGGOO_ENDIAN_LITTLE
    #endif

} // namespace AggOO

#if ( AGGOO_PLATFORM == AGGOO_PLATFORM_WIN32 )
#   include <windows.h>
#endif

#endif  // AggOOPlatform_H_

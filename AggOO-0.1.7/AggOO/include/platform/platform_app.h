
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
// $Id: platform_app.h,v 1.1 2006/11/01 22:41:01 dratek Exp $
//
// platform_app.h -- Header file for the platform_app class.
//
// Author       : Chad M. Draper
// Date         : August 31, 2006
// Description  : Class which can be used as a base class for demo applications.
//                This should handle all of the platform-specific elements for
//                creating and running an application.
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2006-08-31      Initial implementation
//      2006-10-03      Removed the platform_app class.  Use the
//                      modified_agg_app instead.
//

#ifndef platform_app_H_
#define platform_app_H_

#include <agg_basics.h>
#include "agg_rendering_buffer.h"
#include "agg_trans_viewport.h"
#include "AggOOPixelFormats.h"
#include "platform/agg_platform_support.h"

namespace AggOO
{
    /** Enumeration of window flags, which describe how the application should
        create the window.
    */
    enum WindowStyle
    {
            WindowStyleResize = 1,
            WindowStyleHWBuffer = 2,
            WindowStyleKeepAspectRatio = 4,
            WindowStyleProcessAllKeys = 8
    };

    /** Flags for the mouse button and the CTRL and ALT keys.
    */
    enum InputFlag
    {
            MouseLeft = 1,
            MouseRight = 2,
            ShiftKey = 4,
            CtrlKey = 8
    };

    /** Key codes.  All keyboard input should be mapped to these values.
        Numeric values are taken from the SDL values.
    */
    enum KeyValue
    {
        // ASCII set. Should be supported everywhere
        KV_Backspace      = 8,
        KV_Tab            = 9,
        KV_Clear          = 12,
        KV_Return         = 13,
        KV_Pause          = 19,
        KV_Escape         = 27,

        // Keypad 
        KV_Delete         = 127,
        KV_KP0            = 256,
        KV_KP1            = 257,
        KV_KP2            = 258,
        KV_KP3            = 259,
        KV_KP4            = 260,
        KV_KP5            = 261,
        KV_KP6            = 262,
        KV_KP7            = 263,
        KV_KP8            = 264,
        KV_KP9            = 265,
        KV_KP_Period      = 266,
        KV_KP_Divide      = 267,
        KV_KP_Multiply    = 268,
        KV_KP_Minus       = 269,
        KV_KP_Plus        = 270,
        KV_KP_Enter       = 271,
        KV_KP_Equals      = 272,

        // Arrow-keys and stuff
        KV_Up             = 273,
        KV_Down           = 274,
        KV_Right          = 275,
        KV_Left           = 276,
        KV_Insert         = 277,
        KV_Home           = 278,
        KV_End            = 279,
        KV_PageUp         = 280,
        KV_PageDown       = 281,

        // Functional keys. You'd better avoid using
        // f11...f15 in your applications if you want 
        // the applications to be portable
        KV_F1             = 282,
        KV_F2             = 283,
        KV_F3             = 284,
        KV_F4             = 285,
        KV_F5             = 286,
        KV_F6             = 287,
        KV_F7             = 288,
        KV_F8             = 289,
        KV_F9             = 290,
        KV_F10            = 291,
        KV_F11            = 292,
        KV_F12            = 293,
        KV_F13            = 294,
        KV_F14            = 295,
        KV_F15            = 296,

        // The possibility of using these keys is 
        // very restricted. Actually it's guaranteed 
        // only in win32_api and win32_sdl implementations
        KV_NumLock        = 300,
        KV_CapsLock       = 301,
        KV_ScrollLock     = 302,

        KV_Count
    };

    /** Slightly modified version of agg::platform_support.  This will eliminate
        the need for platform_app, since we can just use the AGG version, but
        I still want/need some modified behavior.
    */
    class modified_agg_app : public agg::platform_support
    {
    protected:
        AggOO::PixelFormat  mAggOOPixelFormat;

    public:
        modified_agg_app( AggOO::PixelFormat pixFormat, bool flipY );

        AggOO::PixelFormat format_aggoo() const { return mAggOOPixelFormat; }

        /** Set the pixel data.
            @remarks
                This sets the raw pixel data, which is converted to the display
                format.
            @param
                buf         Pointer to the raw pixel data
            @param
                width       Width of the raw buffer
            @param
                height      Height of the raw buffer
            @param
                pixFmt      Format of the raw buffer
        */
        void setPixelData( unsigned char* buf, size_t width, size_t height, AggOO::PixelFormat pixFmt );

        /** Post the quit message.  Basically, the implementation doesn't need
            to know about the operating system, but if on_key is overridden,
            then it needs to provide a way to quit the application with the
            escape key (if the implementor desires it.)  Since the app doesn't
            know about the OS, it doesn't know how to send the quit message.
            Instead, it should call this function, which DOES know about the OS
            and can handle the operation.
        */
        void postQuit( int code );

        virtual void on_key( int x, int y, unsigned int key, unsigned int flags );
    };

} // namespace AggOO


#endif  // platform_app_H_


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
// $Id: platform_app.cpp,v 1.1 2006/11/01 22:41:15 dratek Exp $
//
// platform_app.cpp -- Implementation file for the platform_app class.
//
// Implements Win32 platform methods

#include "AggOOPlatform.h"
#if ( AGGOO_PLATFORM == AGGOO_PLATFORM_WIN32 )
#include <windows.h>
#endif

#include <string.h>
#include "platform/platform_app.h"
//#include "util/agg_color_conv.h"
#include "util/agg_color_conv_rgb8.h"
#include "util/agg_color_conv_rgb16.h"
#include "agg_color_rgba.h"

#include "AggOOImaging.h"
#include "AggOOPixelFormatConverter.h"

namespace AggOO
{

    //==========================================================================
    // class modified_agg_app
    modified_agg_app::modified_agg_app( AggOO::PixelFormat pixFormat, bool flipY )
        : mAggOOPixelFormat( pixFormat ),
          agg::platform_support( agg::pix_format_e( AggOO::PixelFormatConverter::convertAggOOtoAGG( pixFormat ) ), flipY )
    {
    }

    //setPixelData--------------------------------------------------------------
    void modified_agg_app::setPixelData( unsigned char* buf, size_t width, size_t height, AggOO::PixelFormat pixFmt )
    {
        int bpp = AggOO::GetPixelFormatSize( pixFmt );
        int rowWidth = AggOO::GetRowWidth( width, bpp );

        // Create a rendering_buffer from the raw data:
        agg::rendering_buffer tempBuf;
/*
        if ( pixFmt != format_aggoo() )
        {
            // Convert the buffer data to a compatible format:
            AggOO::BitmapData origBuf, convBuf;
            int convRowWidth = AggOO::GetRowWidth( width, AggOO::GetPixelFormatSize( format_aggoo() ) );
            unsigned char* buf2 = new unsigned char [ convRowWidth * height ];
            origBuf.Width   = width;
            origBuf.Height  = height;
            origBuf.Stride  = rowWidth;
            origBuf.PixelFormat = pixFmt;
            origBuf.Scan0   = buf;

            convBuf.Width   = width;
            convBuf.Height  = height;
            convBuf.Stride  = convRowWidth;
            convBuf.PixelFormat = format_aggoo();
            convBuf.Scan0   = buf2;

            AggOO::BufferConverter::ConvertPixelFormat( &origBuf, &convBuf );

            // Create the rendering buffer:
            tempBuf.attach( buf2, width, height, ( flip_y() ) ? -convRowWidth : convRowWidth );

            // Copy the temp buffer to the window:
            rbuf_window().copy_from( tempBuf );

            delete [] buf2;
        }
        else
        {
            tempBuf.attach( buf, width, height, ( flip_y() ) ? -rowWidth : rowWidth );

            // Copy the temp buffer to the window:
            rbuf_window().copy_from( tempBuf );
        }
*/
            tempBuf.attach( buf, width, height, ( flip_y() ) ? -rowWidth : rowWidth );

            // Copy the temp buffer to the window:
            rbuf_window().copy_from( tempBuf );

        force_redraw();
    }

    //on_key--------------------------------------------------------------------
    void modified_agg_app::on_key( int x, int y, unsigned int key, unsigned int flags )
    {
        switch ( key )
        {
        case agg::key_escape:
#if ( AGGOO_PLATFORM == AGGOO_PLATFORM_WIN32 )
            ::PostQuitMessage( 0 );
#endif
            break;
        }
    }

} // namespace AggOO


//==============================================================================

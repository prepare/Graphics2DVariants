
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

/*! $Id: AggOOGraphicsPathCompound.cpp,v 1.4 2009/01/15 15:44:49 dratek Exp $
 *  @file   AggOOGraphicsPathCompound.cpp
 *  
 */

#include "AggOOBrush.h"
#include "AggOOPen.h"
#include "AggOOGraphicsPath.h"
#include "AggOOGraphicsPathCompound.h"
#include "AggOOColorExt.h"

#include "AggOOStyleHandler.h"
#include "AggOOStyleHandlerGradient.h"
#include "AggOOStyleHandlerImage.h"

namespace AggOO
{
    GraphicsPathCompound::GraphicsPathCompound(void)
    {
    }

    GraphicsPathCompound::~GraphicsPathCompound(void)
    {
    }

    //createStyleHandler--------------------------------------------------------
    AggOO::StyleHandler* GraphicsPathCompound::createStyleHandler( const Brush* brush )
    {
        AggOO::StyleHandler* handler = NULL;
        switch ( brush->GetType() )
        {
        case BrushTypeTextureFill:
            {
                //handler = new SolidStyleHandler<agg::rgba8>( ColorExt::toRGBA8( Color::Red ) );
                handler = new SolidStyleHandler( ColorExt::toRGBA8( Color::Red ) );
            }
            break;

        case BrushTypeLinearGradient:
            {
                //handler = new SolidStyleHandler<agg::rgba8>( ColorExt::toRGBA8( Color::Green ) );
                handler = new SolidStyleHandler( ColorExt::toRGBA8( Color::Green ) );
            }
            break;

        case BrushTypeSolidColor:
            {
                Color col;
                ((SolidBrush*)brush)->GetColor( &col );
                //handler = new SolidStyleHandler<agg::rgba8>( ColorExt::toRGBA8( col ) );
                handler = new SolidStyleHandler( ColorExt::toRGBA8( col ) );
            }
            break;

            // Need to add hatch and path gradient

        default:
            {
                // This is more of a fail-safe.  If something goes wrong, a solid
                // style handler will be created using a transparent color.  This
                // won't render anything, but it will prevent the application
                // from crashing.
                //handler = new SolidStyleHandler< agg::rgba8 >( ColorExt::toRGBA8( Color::Blue ) );
                handler = new SolidStyleHandler( ColorExt::toRGBA8( Color::Blue ) );
            }
            break;
        }
/*
*/

        return handler;
    }

    //AddPath-------------------------------------------------------------------
    Status GraphicsPathCompound::AddPath( const Pen* pen, const GraphicsPath* path )
    {
        CompoundItem newItem;
        newItem.first   = StylePtr( createStyleHandler( pen->GetBrush() ) );
        newItem.second  = PathPtr( path->Clone() );
        mPaths.push_back( newItem );

        return SetStatus( Ok );
    }

    //AddPath-------------------------------------------------------------------
    Status GraphicsPathCompound::AddPath( const Brush* brush, const GraphicsPath* path )
    {
        CompoundItem newItem;
        newItem.first   = StylePtr( createStyleHandler( brush ) );
        newItem.second  = PathPtr( path->Clone() );
        mPaths.push_back( newItem );

        return SetStatus( Ok );
    }

    //GetPathCount--------------------------------------------------------------
    UINT GraphicsPathCompound::GetPathCount() const
    {
        return mPaths.size();
    }

    //GetStyleHandler-----------------------------------------------------------
    Status GraphicsPathCompound::GetStyleHandler( INT index, StyleHandler* &handler ) const
    {
        if ( index < 0 || index >= mPaths.size() )
            return SetStatus( InvalidParameter );
        try
        {
            handler = mPaths.at( index ).first.getPointer();
            return SetStatus( Ok );
        }
        catch ( std::exception& e )
        {
            return SetStatus( GenericError );
        }

        return SetStatus( Ok );
    }

    //GetGraphicsPath-----------------------------------------------------------
    Status GraphicsPathCompound::GetGraphicsPath( INT index, GraphicsPath* &path ) const
    {
        if ( index < 0 || index >= mPaths.size() )
            return SetStatus( InvalidParameter );
        try
        {
            path = mPaths.at( index ).second.get();
            return SetStatus( Ok );
        }
        catch ( std::exception& e )
        {
            return SetStatus( GenericError );
        }

        return SetStatus( Ok );
    }

    ////GetStyleHandler-----------------------------------------------------------
    //agg::StyleHandler* GraphicsPathCompound::GetStyleHandler( INT index ) const
    //{
    //    //try
    //    //{
    //    //    return mPaths.at( index ).first.get();
    //    //}
    //    //catch ( std::exception& e )
    //    //{
    //    //    return NULL;
    //    //}
    //    return NULL;
    //}

    ////GetGraphicsPath-----------------------------------------------------------
    //GraphicsPath* GraphicsPathCompound::GetGraphicsPath( INT index ) const
    //{
    //    //try
    //    //{
    //    //    return mPaths.at( index ).second.get();
    //    //}
    //    //catch ( std::exception& e )
    //    //{
    //    //    return NULL;
    //    //}
    //    return NULL;
    //}

} // namespace AggOO

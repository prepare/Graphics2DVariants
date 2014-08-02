
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

/*! $Id: AggOOVectorShapePath.cpp,v 1.2 2008/02/28 20:55:50 dratek Exp $
 *  @file   AggOOVectorShapePath.h
 *  @author Chad M. Draper
 *  @date   November 27, 2007
 *  
 */

#include "AggOOVectorShapePath.h"
#include "AggOOEnums.h"
#include "AggOOColorExt.h"
#include "AggOOMatrix.h"
#include "AggOOGraphicsExt.h"
#include "AggOOBrush.h"
#include "AggOOPen.h"
#include "AggOOGraphicsPathExt.h"

namespace AggOO
{
    namespace vs
    {
        VectorShapePath::VectorShapePath(void)
            : mFlipHorizontal( false ),
              mFlipVertical( false )
        {
        }

        VectorShapePath::~VectorShapePath(void)
        {
        }

        //Reset---------------------------------------------------------------------
        Status VectorShapePath::Reset()
        {
            mPath.remove_all();
            return SetStatus( Ok );
        }

        //MoveTo--------------------------------------------------------------------
        Status VectorShapePath::MoveTo( REAL x, REAL y, bool rel )
        {
            mPath.move_to( x, y, rel );
            return SetStatus( Ok );
        }

        //LineTo--------------------------------------------------------------------
        Status VectorShapePath::LineTo( REAL x, REAL y, bool rel )
        {
            mPath.line_to( x, y, rel );
            return SetStatus( Ok );
        }

        //HLineTo-------------------------------------------------------------------
        Status VectorShapePath::HLineTo( REAL x, bool rel )
        {
            mPath.hline_to( x, rel );
            return SetStatus( Ok );
        }

        //VLineTo-------------------------------------------------------------------
        Status VectorShapePath::VLineTo( REAL y, bool rel )
        {
            mPath.vline_to( y, rel );
            return SetStatus( Ok );
        }

        //Curve3--------------------------------------------------------------------
        Status VectorShapePath::Curve3( REAL x1, REAL y1, REAL x, REAL y, bool rel )
        {
            mPath.curve3( x1, y1, x, y, rel );
            return SetStatus( Ok );
        }

        //Curve3--------------------------------------------------------------------
        Status VectorShapePath::Curve3( REAL x, REAL y, bool rel )
        {
            mPath.curve3( x, y, rel );
            return SetStatus( Ok );
        }

        //Curve4--------------------------------------------------------------------
        Status VectorShapePath::Curve4( REAL x1, REAL y1, REAL x2, REAL y2, REAL x, REAL y, bool rel )
        {
            mPath.curve4( x1, y1, x2, y2, x, y, rel );
            return SetStatus( Ok );
        }

        //Curve4--------------------------------------------------------------------
        Status VectorShapePath::Curve4( REAL x2, REAL y2, REAL x, REAL y, bool rel )
        {
            mPath.curve4( x2, y2, x, y, rel );
            return SetStatus( Ok );
        }

        //CloseSubPath--------------------------------------------------------------
        Status VectorShapePath::CloseSubPath()
        {
            mPath.close_subpath();
            return SetStatus( Ok );
        }

        //Fill----------------------------------------------------------------------
        Status VectorShapePath::SetFill( const Color& col )
        {
            mPath.fill( ColorExt::toRGBA8( col ) );
            return SetStatus( Ok );
        }

        //Stroke--------------------------------------------------------------------
        Status VectorShapePath::SetStroke( const Color& col )
        {
            mPath.stroke( ColorExt::toRGBA8( col ) );
            return SetStatus( Ok );
        }

        //EvenOdd-------------------------------------------------------------------
        Status VectorShapePath::SetEvenOdd( bool flag )
        {
            mPath.even_odd( flag );
            return SetStatus( Ok );
        }

        //StrokeWidth---------------------------------------------------------------
        Status VectorShapePath::SetStrokeWidth( REAL w )
        {
            mPath.stroke_width( w );
            return SetStatus( Ok );
        }

        //FillNone------------------------------------------------------------------
        Status VectorShapePath::SetFillNone()
        {
            mPath.fill_none();
            return SetStatus( Ok );
        }

        //StrokeNone----------------------------------------------------------------
        Status VectorShapePath::SetStrokeNone()
        {
            mPath.stroke_none();
            return SetStatus( Ok );
        }

        //FillOpacity---------------------------------------------------------------
        Status VectorShapePath::SetFillOpacity( REAL op )
        {
            mPath.fill_opacity( op );
            return SetStatus( Ok );
        }

        //StrokeOpacity-------------------------------------------------------------
        Status VectorShapePath::SetStrokeOpacity( REAL op )
        {
            mPath.stroke_opacity( op );
            return SetStatus( Ok );
        }

        //LineJoin------------------------------------------------------------------
        Status VectorShapePath::SetLineJoin( LineJoin join )
        {
            mPath.line_join( convertLineJoin( join ) );
            return SetStatus( Ok );
        }

        //LineCap-------------------------------------------------------------------
        Status VectorShapePath::SetLineCap( LineCap cap )
        {
            mPath.line_cap( convertLineCap( cap ) );
            return SetStatus( Ok );
        }

        //MiterLimit----------------------------------------------------------------
        Status VectorShapePath::SetMiterLimit( REAL limit )
        {
            mPath.miter_limit( limit );
            return SetStatus( Ok );
        }

        //Transform-----------------------------------------------------------------
        Matrix VectorShapePath::Transform()
        {
            Matrix mtx;
            mtx.SetAffine( mPath.transform() );
            return mtx;
        }

        //ArrangeOrientations-------------------------------------------------------
        Status VectorShapePath::ArrangeOrientations()
        {
            mPath.arrange_orientations();
            return SetStatus( Ok );
        }

        //Expand--------------------------------------------------------------------
        Status VectorShapePath::Expand( REAL value )
        {
            mPath.expand( value );
            return SetStatus( Ok );
        }

        //Render----------------------------------------------------------------
        void VectorShapePath::Render( Graphics* gfx )
        {
            // Get all of the shapes:
            std::vector< VectorShapeData > shapes;
            Matrix mtx;
            RenderShapeData( mtx, shapes );

            // Go over the shapes and render each one to the display
            std::vector< VectorShapeData >::const_iterator iter;
            AggOO::GraphicsPathExt tempPath;
            for ( iter = shapes.begin(); iter != shapes.end(); iter++ )
            {
                tempPath.Reset();
                tempPath.AddVectorShape( *iter );

                if ( iter->GetFillFlag() )
                {
                    //tempPath.SetFillMode( AggOO::FillModeWinding );
                    tempPath.SetFillMode( iter->IsEvenOddFill() ? AggOO::FillModeAlternate : AggOO::FillModeWinding );
                    AggOO::SolidBrush br( iter->GetFillColor() );
                    gfx->FillPath( &br, &tempPath );
                }

                if ( iter->GetStrokeFlag() )
                {
                    AggOO::Pen pen( iter->GetStrokeColor(), iter->GetStrokeWidth() );
                    gfx->DrawPath( &pen, &tempPath );
                }
            }
        }

    } //namespace vs
} // namespace AggOO

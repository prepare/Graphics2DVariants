
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
// $Id: AggOOAIPath.cpp,v 1.2 2007/09/06 21:50:43 dratek Exp $
//
// AggOOAIPath.cpp
//

#include "aggooaipath.h"
#include "AggOOGraphics.h"
#include "AggOOBrush.h"
#include "AggOOPen.h"
#include "AggOOGraphicsPathExt.h"

namespace AggOO
{
    namespace ai
    {
        AIPath::AIPath(void)
        {
        }

        AIPath::~AIPath(void)
        {
        }

        //Reset---------------------------------------------------------------------
        Status AIPath::Reset()
        {
            mPath.remove_all();
            return SetStatus( Ok );
        }

        //MoveTo--------------------------------------------------------------------
        Status AIPath::MoveTo( REAL x, REAL y, bool rel )
        {
            mPath.move_to( x, y, rel );
            return SetStatus( Ok );
        }

        //LineTo--------------------------------------------------------------------
        Status AIPath::LineTo( REAL x, REAL y, bool rel )
        {
            mPath.line_to( x, y, rel );
            return SetStatus( Ok );
        }

        //HLineTo-------------------------------------------------------------------
        Status AIPath::HLineTo( REAL x, bool rel )
        {
            mPath.hline_to( x, rel );
            return SetStatus( Ok );
        }

        //VLineTo-------------------------------------------------------------------
        Status AIPath::VLineTo( REAL y, bool rel )
        {
            mPath.vline_to( y, rel );
            return SetStatus( Ok );
        }

        //Curve3--------------------------------------------------------------------
        Status AIPath::Curve3( REAL x1, REAL y1, REAL x, REAL y, bool rel )
        {
            mPath.curve3( x1, y1, x, y, rel );
            return SetStatus( Ok );
        }

        //Curve3--------------------------------------------------------------------
        Status AIPath::Curve3( REAL x, REAL y, bool rel )
        {
            mPath.curve3( x, y, rel );
            return SetStatus( Ok );
        }

        //Curve4--------------------------------------------------------------------
        Status AIPath::Curve4( REAL x1, REAL y1, REAL x2, REAL y2, REAL x, REAL y, bool rel )
        {
            mPath.curve4( x1, y1, x2, y2, x, y, rel );
            return SetStatus( Ok );
        }

        //Curve4--------------------------------------------------------------------
        Status AIPath::Curve4( REAL x2, REAL y2, REAL x, REAL y, bool rel )
        {
            mPath.curve4( x2, y2, x, y, rel );
            return SetStatus( Ok );
        }

        //CloseSubPath--------------------------------------------------------------
        Status AIPath::CloseSubPath()
        {
            mPath.close_subpath();
            return SetStatus( Ok );
        }

        //Fill----------------------------------------------------------------------
        Status AIPath::SetFill( const Color& col )
        {
            mPath.fill( ColorExt::convertColor( col ) );
            return SetStatus( Ok );
        }

        //Stroke--------------------------------------------------------------------
        Status AIPath::SetStroke( const Color& col )
        {
            mPath.stroke( ColorExt::convertColor( col ) );
            return SetStatus( Ok );
        }

        //EvenOdd-------------------------------------------------------------------
        Status AIPath::SetEvenOdd( bool flag )
        {
            mPath.even_odd( flag );
            return SetStatus( Ok );
        }

        //StrokeWidth---------------------------------------------------------------
        Status AIPath::SetStrokeWidth( REAL w )
        {
            mPath.stroke_width( w );
            return SetStatus( Ok );
        }

        //FillNone------------------------------------------------------------------
        Status AIPath::SetFillNone()
        {
            mPath.fill_none();
            return SetStatus( Ok );
        }

        //StrokeNone----------------------------------------------------------------
        Status AIPath::SetStrokeNone()
        {
            mPath.stroke_none();
            return SetStatus( Ok );
        }

        //FillOpacity---------------------------------------------------------------
        Status AIPath::SetFillOpacity( REAL op )
        {
            mPath.fill_opacity( op );
            return SetStatus( Ok );
        }

        //StrokeOpacity-------------------------------------------------------------
        Status AIPath::SetStrokeOpacity( REAL op )
        {
            mPath.stroke_opacity( op );
            return SetStatus( Ok );
        }

        //LineJoin------------------------------------------------------------------
        Status AIPath::SetLineJoin( LineJoin join )
        {
            mPath.line_join( convertLineJoin( join ) );
            return SetStatus( Ok );
        }

        //LineCap-------------------------------------------------------------------
        Status AIPath::SetLineCap( LineCap cap )
        {
            mPath.line_cap( convertLineCap( cap ) );
            return SetStatus( Ok );
        }

        //MiterLimit----------------------------------------------------------------
        Status AIPath::SetMiterLimit( REAL limit )
        {
            mPath.miter_limit( limit );
            return SetStatus( Ok );
        }

        //Transform-----------------------------------------------------------------
        Matrix AIPath::Transform()
        {
            Matrix mtx;
            mtx.SetAffine( mPath.transform() );
            return mtx;
        }

        //ArrangeOrientations-------------------------------------------------------
        Status AIPath::ArrangeOrientations()
        {
            mPath.arrange_orientations();
            return SetStatus( Ok );
        }

        //Expand--------------------------------------------------------------------
        Status AIPath::Expand( REAL value )
        {
            mPath.expand( value );
            return SetStatus( Ok );
        }

        //Render----------------------------------------------------------------
        void AIPath::Render( Graphics* gfx )
        {
            // Get all of the shapes:
            std::vector< AggOO::ai::AIShapeData > shapes;
            Matrix mtx;
            RenderShapeData( mtx, shapes );

            // Go over the shapes and render each one to the display
            std::vector< AggOO::ai::AIShapeData >::const_iterator iter;
            AggOO::GraphicsPathExt tempPath;
            for ( iter = shapes.begin(); iter != shapes.end(); iter++ )
            {
                tempPath.Reset();
                tempPath.AddAIShape( *iter );

                if ( iter->GetFillFlag() )
                {
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


    } // namespace ai
} // namespace AggOO

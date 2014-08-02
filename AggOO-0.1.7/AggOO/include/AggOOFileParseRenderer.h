
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
// $Id: AggOOFileParseRenderer.h,v 1.4 2009/02/26 22:13:38 dratek Exp $
//
// AggOOFileParseRenderer.h -- Renders vector files parsed by the FileParse
//                             library using AggOO.
//
// Author       : Chad M. Draper
// Date         : October 17, 2008
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2008-10-17      Initial implementation
//

#ifndef AggOOFileParseRenderer_H_
#define AggOOFileParseRenderer_H_

#include "AggOOConfig.h"

#if ( AGGOO_ENABLE_VECTOR_LOADING == 1 )

#include "AggOOColor.h"

#include "FileParseRenderer.h"
#include "FileParseColor.h"
#include "FileParseColorStyles.h"
#include "FileParseMatrix.h"

namespace FileParse
{
    class Path;
    class FillStyle;
    class LineStyle;
}

namespace AggOO
{
    class Brush;
    class Graphics;

    //namespace FileParse
    //{
        /** @class RendererAggOO
            A sub-class of FileParse::RendererBase, which provides for rendering
            a vector file using AggOO.
        */
        class _AggOOExport RendererAggOO : public ::FileParse::RendererBase
        {
        protected:
            ::AggOO::Graphics*    mSurface;         ///< Rendering surface.  Must be set in the constructor
            //::AggOO::RectF        mBounds;

            /** Render a path */
            virtual void RenderPath( const ::FileParse::Path* path, const ::FileParse::FillStyle* fillStyle0, const ::FileParse::FillStyle* fillStyle1, const ::FileParse::LineStyle* lineStyle ) const;

            /** Add a FileParse::Path to an AggOO::GraphicsPath */
            virtual void AddShapePath( ::AggOO::GraphicsPath* gfxPath, const ::FileParse::Path* fpPath ) const;

            /** Render a path */
            virtual void RenderPath( const ::AggOO::GraphicsPath* gfxPath, const ::FileParse::FillStyle* fillStyle0, const ::FileParse::FillStyle* fillStyle1, const ::FileParse::LineStyle* lineStyle ) const;

            /** Add a path to a compound path */
            virtual void AddPathToCompound( ::AggOO::GraphicsPathCompound* compound, const ::FileParse::Path* path, const ::FileParse::FillStyle* fillStyle0, const ::FileParse::FillStyle* fillStyle1, const ::FileParse::LineStyle* lineStyle ) const;

        protected:

            /** Create a brush from a given fill style */
            virtual ::AggOO::Brush* CreateBrush( const ::FileParse::FillStyle* style ) const;

            /** Create a pen from a given line style */
            virtual ::AggOO::Pen* CreatePen( const ::FileParse::LineStyle* style ) const;

            /** Clear background */
            void ClearBackground( const ::FileParse::Color& color = ::FileParse::Color::Transparent ) const;

            /** Transform the renderer by a matrix */
            void Transform( const ::FileParse::Matrix* mtx );

            /** Render a shape */
            void RenderShape( const ::FileParse::ShapeDef* shape ) const;

        public:

            /** Create the renderer, passing the surface on which the image will
                be renderered.
            */
            RendererAggOO( ::AggOO::Graphics* gfx );

            /** Destructor */
            virtual ~RendererAggOO(void);

        }; // class RendererAggOO

    //} // namespace FileParse
} // namespace AggOO

#endif  // AGGOO_ENABLE_VECTOR_LOADING == 1
#endif  // AggOOFileParseRenderer_H_

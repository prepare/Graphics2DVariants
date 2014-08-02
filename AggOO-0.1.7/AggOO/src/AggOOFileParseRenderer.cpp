
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
// $Id: AggOOFileParseRenderer.cpp,v 1.5 2009/03/13 23:00:31 dratek Exp $
//
// AggOOFileParseRenderer.cpp
//
// Author       : Chad M. Draper
// Date         : October 17, 2008
//

#include "AggOOFileParseRenderer.h"


#if ( AGGOO_ENABLE_VECTOR_LOADING == 1 )

#include "AggOOGraphics.h"
#include "AggOOGraphicsPath.h"
#include "AggOOGraphicsPathCompound.h"
#include "AggOOPen.h"
#include "AggOOBrush.h"

#include "FileParseTypes.h"
#include "FileParseColor.h"
#include "FileParseColorStyles.h"
#include "FileParseGradient.h"
#include "FileParseShapeDef.h"

namespace AggOO
{
    //namespace FileParse
    //{
        //Constructor-----------------------------------------------------------
        RendererAggOO::RendererAggOO( ::AggOO::Graphics* gfx )
            : mSurface( gfx )
        {
        }

        //Destructor------------------------------------------------------------
        RendererAggOO::~RendererAggOO(void)
        {
        }

        //CreateBrush-----------------------------------------------------------
        ::AggOO::Brush* RendererAggOO::CreateBrush( const ::FileParse::FillStyle* style ) const
        {
            if ( !style )   return 0;
            ::FileParse::FillStyle* fillStyle = const_cast< ::FileParse::FillStyle* >( style );
            if ( style->IsGradient() )
            {
                ::AggOO::LinearGradientBrushExt* br = 0;
                ::FileParse::Gradient* gradient = 0;
                if ( style->GetFillMode() == ::FileParse::FillStyle::GradientLinear )
                {
                    ::FileParse::LinearGradientFillStyle* gradStyle = static_cast< ::FileParse::LinearGradientFillStyle* >( fillStyle );
                    gradient = gradStyle->mGradient.Get();
                    ::FileParse::LinearGradient* linearGradient = static_cast< ::FileParse::LinearGradient* >( gradient );

                    ::FileParse::Point2Df origin = linearGradient->GetOrigin();
                    ::FileParse::Point2Df endPos = linearGradient->GetEndPosition();
                    br = new ::AggOO::LinearGradientBrushExt( ::AggOO::PointF( origin.x, origin.y ), ::AggOO::PointF( endPos.x, endPos.y ), ::AggOO::Color::Transparent, ::AggOO::Color::Transparent );

                    br->SetGradientInterpolationMode( ( gradient->GetInterpolationMode() == ::FileParse::Gradient::InterpLinear ) ? ::AggOO::GradientInterpolationLinear : ::AggOO::GradientInterpolationStep );
                    br->SetGradientInterpolationMode( ::AggOO::GradientInterpolationLinear );
                    br->SetWrapMode( ::AggOO::WrapModeFlip );
                }
                else // radial or focal gradient
                {
                    ::FileParse::RadialGradientFillStyle* gradStyle = static_cast< ::FileParse::RadialGradientFillStyle* >( fillStyle );
                    gradient = gradStyle->mGradient.Get();
                    ::FileParse::RadialGradient* radialGradient = static_cast< ::FileParse::RadialGradient* >( gradient );

                    ::FileParse::Point2Df origin = radialGradient->GetOrigin();
                    ::FileParse::Point2Df endPos = origin + ::FileParse::Point2Df( radialGradient->GetRadius(), 0.0 );
                    br = new ::AggOO::LinearGradientBrushExt( ::AggOO::PointF( origin.x, origin.y ), ::AggOO::PointF( endPos.x, endPos.y ), ::AggOO::Color::Transparent, ::AggOO::Color::Transparent );
                    br->SetGradientStyle( ::AggOO::GradientStyleRadial );

                    br->SetGradientInterpolationMode( ( gradient->GetInterpolationMode() == ::FileParse::Gradient::InterpLinear ) ? ::AggOO::GradientInterpolationLinear : ::AggOO::GradientInterpolationStep );
                    br->SetGradientInterpolationMode( ::AggOO::GradientInterpolationLinear );
                    br->SetWrapMode( ::AggOO::WrapModeFlip );
                }

                std::vector< ::FileParse::Real > aggGradientPos;
                std::vector< ::AggOO::Color > aggGradientCol;
                for ( ::FileParse::Int i = 0; i < gradient->NumItems(); i++ )
                {
                    // Get the gradient item:
                    ::FileParse::GradientItem gradItem = gradient->GetAt( i );
                    ::FileParse::Color gradColor = gradItem.GetColor() * 255.0;

                    // Create and fill in the color item, converting to an
                    // AggOO color...
                    aggGradientPos.push_back( gradItem.GetPosition() );
                    aggGradientCol.push_back( ::AggOO::Color( gradColor.a, gradColor.r, gradColor.g, gradColor.b ) );
                }

                // If there were no items in the gradient, just use a transparent color:
                if ( gradient->NumItems() == 0 )
                {
                    aggGradientPos.push_back( 0.0 );
                    aggGradientCol.push_back( ::AggOO::Color::Transparent );
                    aggGradientPos.push_back( 1.0 );
                    aggGradientCol.push_back( ::AggOO::Color::Transparent );
                }

                // If there is just one item in the gradient, add a duplicate of that item
                else if ( gradient->NumItems() == 1 )
                {
                    aggGradientPos.push_back( 1.0 );
                    aggGradientCol.push_back( aggGradientCol.front() );
                }

                br->SetInterpolationColors( &aggGradientCol[ 0 ], &aggGradientPos[ 0 ], aggGradientPos.size() );
                return br;
            }
            else
            {
                ::FileParse::SolidFillStyle* solid = static_cast< ::FileParse::SolidFillStyle* >( fillStyle );
                ::AggOO::SolidBrush* br = new ::AggOO::SolidBrush( ::AggOO::Color( solid->mColor.a * 255, solid->mColor.r * 255, solid->mColor.g * 255, solid->mColor.b * 255 ) );
                return br;
            }
            return 0;
        }

        //CreatePen-------------------------------------------------------------
        ::AggOO::Pen* RendererAggOO::CreatePen( const ::FileParse::LineStyle* style ) const
        {
            if ( !style )   return 0;
            const ::AggOO::Brush* pathBr = CreateBrush( style->GetFillStyle() );

            ::AggOO::Pen* pathPen = new ::AggOO::Pen( pathBr, style->GetWidth() );
            pathPen->SetLineJoin( ( style->GetLineJoin() == ::FileParse::LineStyle::JoinRound ) ? ::AggOO::LineJoinRound : ( style->GetLineJoin() == ::FileParse::LineStyle::JoinBevel ) ? ::AggOO::LineJoinBevel : ::AggOO::LineJoinMiter );
            pathPen->SetMiterLimit( style->GetMiterLimit() );
            pathPen->SetStartCap( ( style->GetStartCap() == ::FileParse::LineStyle::CapRound ) ? ::AggOO::LineCapRound : ( style->GetStartCap() == ::FileParse::LineStyle::CapSquare ) ? ::AggOO::LineCapSquare : ::AggOO::LineCapFlat );
            pathPen->SetEndCap( ( style->GetEndCap() == ::FileParse::LineStyle::CapRound ) ? ::AggOO::LineCapRound : ( style->GetEndCap() == ::FileParse::LineStyle::CapSquare ) ? ::AggOO::LineCapSquare : ::AggOO::LineCapFlat );

            delete pathBr;
            return pathPen;
        }

        //RenderPath------------------------------------------------------------
        void RendererAggOO::RenderPath( const ::FileParse::Path* path, const ::FileParse::FillStyle* fillStyle0, const ::FileParse::FillStyle* fillStyle1, const ::FileParse::LineStyle* lineStyle ) const
        {
            // Render line segments defining the path.
            ::FileParse::Point2Df prevPt = path->GetOrigin();
            ::AggOO::GraphicsPath gfxPath;

            for ( int i = 0; i < path->Size(); i++ )
            {
                const ::FileParse::Edge& edge = (*path)[ i ];
                const ::FileParse::Point2Df& nextPt = edge.GetAnchorPoint();

                if ( edge.IsStraight() )
                    gfxPath.AddLine( prevPt.x, prevPt.y, nextPt.x, nextPt.y );
                else
                {
                    const ::FileParse::Point2Df& ctrlPt = edge.GetControlPoint1();
                    if ( edge.IsCubic() )
                    {
                        const ::FileParse::Point2Df& ctrlPt2 = edge.GetControlPoint2();
                        gfxPath.AddBezier( prevPt.x, prevPt.y, ctrlPt.x, ctrlPt.y, ctrlPt2.x, ctrlPt2.y, nextPt.x, nextPt.y );
                    }
                    else
                        gfxPath.AddQuadratic( prevPt.x, prevPt.y, ctrlPt.x, ctrlPt.y, nextPt.x, nextPt.y );
                }

                prevPt = nextPt;
            }

            if ( path->IsClosed() )
                gfxPath.CloseFigure();

            //// Get the matrix used for the path, and convert it to an AggOO matrix
            ////::AggOO::GraphicsPathCompound compound;
            //::AggOO::SolidBrush br( ::AggOO::Color( 255, 255, 255, 255 ) );
            //mSurface->FillPath( &br, &gfxPath );

            if ( lineStyle )
            {
                const ::AggOO::Pen* pen = CreatePen( lineStyle );

                //lfm << "Line Pen = " << pathPen << std::endl;
                //compound.AddPath( &pathPen, &gfxPath );
                //mSurface->RenderCompoundPath( &compound );
                mSurface->DrawPath( pen, &gfxPath );
                delete pen;
            }
            if ( fillStyle0 )
            {
                const ::AggOO::Brush* fillBr = CreateBrush( fillStyle0 );
                //lfm << "Brush 0 = " << *fillBr << std::endl;
                //compound.AddPath( fillBr, &gfxPath );
                //mSurface->RenderCompoundPath( &compound );
                mSurface->FillPath( fillBr, &gfxPath );
                delete fillBr;
            }
            if ( fillStyle1 )
            {
                const ::AggOO::Brush* fillBr = CreateBrush( fillStyle1 );
                //lfm << "Brush 1 = " << *fillBr << std::endl;
                //compound.AddPath( fillBr, &gfxPath );
                //mSurface->RenderCompoundPath( &compound );
                mSurface->FillPath( fillBr, &gfxPath );
                delete fillBr;
            }
            //mSurface->RenderCompoundPath( &compound );
        }

        //AddShapePath----------------------------------------------------------
        void RendererAggOO::AddShapePath( ::AggOO::GraphicsPath* gfxPath, const ::FileParse::Path* fpPath ) const
        {
            // Add the FileParse::Path to the AggOO::GraphicsPath
            ::FileParse::Point2Df prevPt = fpPath->GetOrigin();
            for ( int i = 0; i < fpPath->Size(); i++ )
            {
                const ::FileParse::Edge& edge = (*fpPath)[ i ];
                const ::FileParse::Point2Df& nextPt = edge.GetAnchorPoint();

                if ( edge.IsStraight() )
                    gfxPath->AddLine( prevPt.x, prevPt.y, nextPt.x, nextPt.y );
                else
                {
                    const ::FileParse::Point2Df& ctrlPt = edge.GetControlPoint1();
                    if ( edge.IsCubic() )
                    {
                        const ::FileParse::Point2Df& ctrlPt2 = edge.GetControlPoint2();
                        gfxPath->AddBezier( prevPt.x, prevPt.y, ctrlPt.x, ctrlPt.y, ctrlPt2.x, ctrlPt2.y, nextPt.x, nextPt.y );
                    }
                    else
                        gfxPath->AddQuadratic( prevPt.x, prevPt.y, ctrlPt.x, ctrlPt.y, nextPt.x, nextPt.y );
                }

                prevPt = nextPt;
            }

            if ( fpPath->IsClosed() )
                gfxPath->CloseFigure();
        }

        //RenderPath------------------------------------------------------------
        void RendererAggOO::RenderPath( const ::AggOO::GraphicsPath* gfxPath, const ::FileParse::FillStyle* fillStyle0, const ::FileParse::FillStyle* fillStyle1, const ::FileParse::LineStyle* lineStyle ) const
        {
            const ::AggOO::Pen* pen = CreatePen( lineStyle );
            if ( pen )
            {
                mSurface->DrawPath( pen, gfxPath );
                delete pen;
            }
            const ::AggOO::Brush* fillBr0 = CreateBrush( fillStyle0 );
            if ( fillBr0 )
            {
                mSurface->FillPath( fillBr0, gfxPath );
                delete fillBr0;
            }
            const ::AggOO::Brush* fillBr1 = CreateBrush( fillStyle1 );
            if ( fillBr1 )
            {
                mSurface->FillPath( fillBr1, gfxPath );
                delete fillBr1;
            }

            //if ( lineStyle )
            //{
            //    const ::AggOO::Pen* pen = CreatePen( lineStyle );

            //    //lfm << "Line Pen = " << pathPen << std::endl;
            //    //compound.AddPath( &pathPen, &gfxPath );
            //    //mSurface->RenderCompoundPath( &compound );
            //    mSurface->DrawPath( pen, gfxPath );
            //    delete pen;
            //}
            //if ( fillStyle0 )
            //{
            //    const ::AggOO::Brush* fillBr = CreateBrush( fillStyle0 );
            //    //lfm << "Brush 0 = " << *fillBr << std::endl;
            //    //compound.AddPath( fillBr, &gfxPath );
            //    //mSurface->RenderCompoundPath( &compound );
            //    mSurface->FillPath( fillBr, gfxPath );
            //    delete fillBr;
            //}
            //if ( fillStyle1 )
            //{
            //    const ::AggOO::Brush* fillBr = CreateBrush( fillStyle1 );
            //    //lfm << "Brush 1 = " << *fillBr << std::endl;
            //    //compound.AddPath( fillBr, &gfxPath );
            //    //mSurface->RenderCompoundPath( &compound );
            //    mSurface->FillPath( fillBr, gfxPath );
            //    delete fillBr;
            //}
        }

        //AddPathToCompound-----------------------------------------------------
        void RendererAggOO::AddPathToCompound( ::AggOO::GraphicsPathCompound* compound, const ::FileParse::Path* path, const ::FileParse::FillStyle* fillStyle0, const ::FileParse::FillStyle* fillStyle1, const ::FileParse::LineStyle* lineStyle ) const
        {
            // Render line segments defining the path.
            ::FileParse::Point2Df prevPt = path->GetOrigin();
            ::AggOO::GraphicsPath gfxPath;

            for ( int i = 0; i < path->Size(); i++ )
            {
                const ::FileParse::Edge& edge = (*path)[ i ];
                const ::FileParse::Point2Df& nextPt = edge.GetAnchorPoint();

                if ( edge.IsStraight() )
                    gfxPath.AddLine( prevPt.x, prevPt.y, nextPt.x, nextPt.y );
                else
                {
                    const ::FileParse::Point2Df& ctrlPt = edge.GetControlPoint1();
                    gfxPath.AddQuadratic( prevPt.x, prevPt.y, ctrlPt.x, ctrlPt.y, nextPt.x, nextPt.y );
                }

                prevPt = nextPt;
            }

            if ( path->IsClosed() )
                gfxPath.CloseFigure();

            if ( lineStyle )
            {
                const ::AggOO::Pen* pen = CreatePen( lineStyle );

                //lfm << "Line Pen = " << pathPen << std::endl;
                compound->AddPath( pen, &gfxPath );
                //mSurface->RenderCompoundPath( &compound );
                //mSurface->DrawPath( &pathPen, &gfxPath );
                delete pen;
            }
            if ( fillStyle0 )
            {
                const ::AggOO::Brush* fillBr = CreateBrush( fillStyle0 );
                //lfm << "Brush 0 = " << *fillBr << std::endl;
                compound->AddPath( fillBr, &gfxPath );
                //mSurface->RenderCompoundPath( &compound );
                //mSurface->FillPath( fillBr, &gfxPath );
                delete fillBr;
            }
            if ( fillStyle1 )
            {
                const ::AggOO::Brush* fillBr = CreateBrush( fillStyle1 );
                //lfm << "Brush 1 = " << *fillBr << std::endl;
                compound->AddPath( fillBr, &gfxPath );
                //mSurface->RenderCompoundPath( &compound );
                //mSurface->FillPath( fillBr, &gfxPath );
                delete fillBr;
            }

            ::AggOO::GraphicsPath path1, path2;
            path1.AddRectangle( ::AggOO::RectF( 0, 0, 100.3, 500 ) );
            ::AggOO::SolidBrush brush( ::AggOO::Color( 255, 0, 255, 200 ) );
            //compound->AddPath( &brush, &path1 );

            path2.AddRectangle( ::AggOO::RectF( 100.3, 0, 120, 500 ) );
            brush.SetColor( ::AggOO::Color( 255, 255, 255, 0 ) );
        }

        //ClearBackground-------------------------------------------------------
        void RendererAggOO::ClearBackground( const ::FileParse::Color& color ) const
        {
            mSurface->Clear( ::AggOO::Color( color.a * 255, color.r * 255, color.g * 255, color.b * 255 ) );
        }

        //Transform-------------------------------------------------------------
        void RendererAggOO::Transform( const ::FileParse::Matrix* mtx )
        {
            ::FileParse::Real tempMat[ 9 ];
            mtx->GetElements( tempMat );
            ::AggOO::Matrix aggooMtx( tempMat[ 0 ], tempMat[ 3 ], tempMat[ 1 ], tempMat[ 4 ], tempMat[ 2 ], tempMat[ 5 ] );
            mSurface->MultiplyTransform( &aggooMtx );
            //mSurface->SetTransform( &aggooMtx );
        }

        //RenderShape-----------------------------------------------------------
        void RendererAggOO::RenderShape( const ::FileParse::ShapeDef* shape ) const
        {
            // Transform the graphics object by the shape's matrix:
            ::AggOO::GraphicsState gfxState = mSurface->Save();
            ::FileParse::Real mtx[ 9 ];
            shape->GetMatrix().GetElements( mtx );
            ::AggOO::Matrix aggooMtx( mtx[ 0 ], mtx[ 1 ], mtx[ 3 ], mtx[ 4 ], mtx[ 2 ], mtx[ 5 ] );
            mSurface->MultiplyTransform( &aggooMtx );

            ::FileParse::RectI bounds = shape->GetShapeBounds();

            // Render the paths:
            ::AggOO::GraphicsPath gfxPath;
            const ::FileParse::FillStyle* fillStyle0 = 0;
            const ::FileParse::FillStyle* fillStyle1 = 0;
            const ::FileParse::LineStyle* lineStyle  = 0;
            ::AggOO::RectF boundingBox;
            int pathCount = shape->GetPathCount();
            for ( ::FileParse::UInt32 pathIndex = 0; pathIndex < shape->GetPathCount(); )
            {
                const ::FileParse::Path& curPath = shape->GetPath( pathIndex );
                ::FileParse::RectF fpPathBounds = curPath.GetBoundingBox();
                ::AggOO::RectF curPathBounds( fpPathBounds.X, fpPathBounds.Y, fpPathBounds.Width, fpPathBounds.Height );
                if ( pathIndex == 0 )
                    boundingBox = curPathBounds;
                else
                    boundingBox.Union( boundingBox, boundingBox, curPathBounds );

                // If this is not the first path in the list (pathIndex > 0) and
                // it is indicated as a first shape, render the current
                // AggOO::GraphicsPath.
                if ( curPath.IsFirstShape() && pathIndex > 0 )
                {
                    RenderPath( &gfxPath, fillStyle0, fillStyle1, lineStyle );
                    gfxPath.Reset();
                }

                fillStyle0 = shape->GetFillStyle( curPath.GetFillStyle0() );
                fillStyle1 = shape->GetFillStyle( curPath.GetFillStyle1() );
                lineStyle  = shape->GetLineStyle( curPath.GetLineStyle() );

                // Increment the path index.  This will let us know if this is
                // the last shape in the list.
                ++pathIndex;

                // Add the current path to the GraphicsPath:
                AddShapePath( &gfxPath, &curPath );

                // If this is the last shape in the list, render the path.
                if ( pathIndex == shape->GetPathCount() )
                    RenderPath( &gfxPath, fillStyle0, fillStyle1, lineStyle );
            }

            //::AggOO::Pen boundsPen( ::AggOO::Color::Cyan, 3 );
            //mSurface->DrawRectangle( &boundsPen, boundingBox );

            mSurface->Restore( gfxState );
        }

    //} // namespace FileParse
} // namespace AggOO

#endif  // AGGOO_ENABLE_VECTOR_LOADING == 1

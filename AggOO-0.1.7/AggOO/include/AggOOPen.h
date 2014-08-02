
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
// $Id: AggOOPen.h,v 1.4 2010/03/12 04:20:57 dratek Exp $
//
// AggOOPen.h -- Pen classes used by AggOO.
//
// Author       : Chad M. Draper
// Date         : September 14, 2006
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2006-09-14      Initial implementation
//

#ifndef AggOOPen_H_
#define AggOOPen_H_

#include "AggOOEnums.h"
#include "AggOOBase.h"
#include "AggOOBrush.h"
#include "AggOOTypes.h"
#include "AggOOPixelFormats.h"
#include "AggOOPrereqs.h"
#include "AggOOColor.h"

namespace AggOO
{
    /** @class Pen

        A Pen is used for drawing lines and curves.
    */
    class _AggOOExport Pen : public Base
    {
    private:
//        PenType     mType;
        LineJoin    mLineJoin;
        LineCap     mStartCap, mEndCap;
        DashCap     mDashCap;
        DashStyle   mDashStyle;
        REAL        mDashOffset;
        REAL        mMiterLimit;
        REAL        mInternalMiterLimit;
        PenAlignment mAlignment;
        Matrix      mMatrix;

    protected:
        mutable Status  mLastStatus;
        REAL        mWidth;
//        Color       mColor;
        Brush*      mBrush;

    public:
        friend class Graphics;
        friend class GraphicsPath;

        //! Prevent default creation
    protected:
        Pen()
        {
            mLineJoin   = LineJoinMiter;
            mStartCap   = LineCapFlat;
            mEndCap     = LineCapFlat;
            mDashCap    = DashCapFlat;
            mDashStyle  = DashStyleSolid;
            mDashOffset = 0;
            mMiterLimit = 10.0;
            mInternalMiterLimit = 1.0;
            mAlignment  = PenAlignmentCenter;

            mBrush = 0;
            //mType = static_cast< PenType >( -1 );
            mLastStatus = SetStatus( Ok );
        }

        //Pen( PenType type, Status status = NotImplemented ) : mBrush( 0 ), mType( type )
        //{
        //    SetStatus( status );
        //}

        //VOID SetType( PenType type )  { mType = type; }

        /** Set the status of the pen
        */
        Status SetStatus( Status status ) const
        {
            if ( status != Ok )
                return ( mLastStatus = status );
            return status;
        }

        //! Prevent copying
    protected:
        Pen( const Pen& src ) { SetBrush( src.GetBrush() ); SetWidth( src.GetWidth() ); }
        Pen& operator=( const Pen& src ) { SetBrush( src.GetBrush() ); SetWidth( src.GetWidth() ); return *this; }

    public:
        /** Constructor */
        Pen( const Color& color, REAL width = 1.0 )
            : mLineJoin( LineJoinMiter ),
              mStartCap( LineCapFlat ), mEndCap( LineCapFlat ),
              mDashCap( DashCapFlat ), mDashStyle( DashStyleSolid ),
              mDashOffset( 0.0 ), mMiterLimit( 10.0 ), mInternalMiterLimit( 1.0 ),
              mAlignment( PenAlignmentCenter )
        {
            mBrush = new SolidBrush( color );
            mWidth = width;
            //mType  = PenTypeSolidColor;

            SetStatus( Ok );
        }
        /** Constructor */
        Pen( const Brush* brush, REAL width = 1.0 )
            : mLineJoin( LineJoinMiter ),
              mStartCap( LineCapFlat ), mEndCap( LineCapFlat ),
              mDashCap( DashCapFlat ), mDashStyle( DashStyleSolid ),
              mDashOffset( 0.0 ), mMiterLimit( 10.0 ), mInternalMiterLimit( 1.0 ),
              mAlignment( PenAlignmentCenter )
        {
            mBrush = brush->Clone();
            mWidth = width;
            //mType  = brush->GetType();

            SetStatus( Ok );
        }
        /** Destructor */
        ~Pen(void)
        {
            delete mBrush;
        }

        /** Retrieve the last status message set by the brush */
        Status GetLastStatus() const    { return mLastStatus; }

        /** Get the brush used by the pen */
        Brush* GetBrush(void) const
        {
            return mBrush;  // Does GDI+ return a clone of the brush?
        }
        /** Set the brush used by the pen */
        Status SetBrush( const Brush* brush )
        {
            if ( mBrush )
                delete mBrush;
            mBrush = brush->Clone();
            if ( mBrush )
                return SetStatus( Ok );
            return SetStatus( OutOfMemory );
        }

        /** Set the pen color */
        Status SetColor( const Color& color )
        {
            if ( mBrush && mBrush->GetType() == BrushTypeSolidColor )
            {
                // Just set the new brush color:
                ((SolidBrush*)mBrush)->SetColor( color );
            }
            else
            {
                // Need to delete the current brush and create a solid brush:
                delete mBrush;
                mBrush = new SolidBrush( color );
            }
            //mColor = color;

            return SetStatus( Ok );
        }
        /** Get the pen color */
        Status GetColor( Color* color ) const
        {
            if ( !color )
                return SetStatus( InvalidParameter );

            if ( mBrush && mBrush->GetType() == BrushTypeSolidColor )
            {
                return SetStatus( ((SolidBrush*)mBrush)->GetColor( color ) );
            }

            return SetStatus( WrongState );
        }

        /** Set the pen width */
        Status SetWidth( REAL width )           { mWidth = width; return SetStatus( Ok ); }
        /** Get the pen width */
        REAL GetWidth(void) const               { return mWidth; }

        /** Create a new clone of the pen */
        Pen* Clone() const;

        /** Retrieve the pen type */
        PenType GetType() const
        {
            if ( mBrush )
                return (PenType)mBrush->GetType();
            return PenTypeUnknown;
        }

        /** Get the line join style */
        LineJoin GetLineJoin(void) const;
        /** Set the line join style */
        Status SetLineJoin( LineJoin lineJoin );

        /** Get the miter limit */
        REAL GetMiterLimit(void) const;
        /** Set the miter limit */
        Status SetMiterLimit( REAL miterLimit );

        /** Gets the internal miter limit (this is not a part of GDI+) */
        REAL GetInternalMiterLimit() const;
        /** Sets the internal miter limit (this is not a part of GDI+) */
        Status SetInternalMiterLimit( REAL miterLimit );

        /** Get the start cap style */
        LineCap GetStartCap(void) const;
        /** Set the start cap style */
        Status SetStartCap( LineCap startCap );

        /** Get the end cap style */
        LineCap GetEndCap(void) const;
        /** Set the end cap style */
        Status SetEndCap( LineCap endCap );

        /** Get the dash cap style */
        DashCap GetDashCap(void) const;
        /** Set the dash cap style */
        Status SetDashCap( DashCap dashCap );

        /** Set the start, end, and dash cap styles */
        Status SetLineCap( LineCap startCap, LineCap endCap, DashCap dashCap );

        /** Get the dash offset */
        REAL GetDashOffset(void) const;
        /** Set the dash offset */
        Status SetDashOffset( REAL dashOffset );

        /** Get the dash style */
        DashStyle GetDashStyle(void) const;
        /** Set the dash style */
        Status SetDashStyle( DashStyle dashStyle );

        /** Translate the pen */
        Status TranslateTransform( REAL dx, REAL dy, MatrixOrder order = MatrixOrderPrepend );
        /** Rotate the pen */
        Status RotateTransform( REAL angle, MatrixOrder order = MatrixOrderPrepend );
        /** Scale the pen */
        Status ScaleTransform( REAL sx, REAL sy, MatrixOrder order = MatrixOrderPrepend );
        /** Multiply the pen's matrix by another matrix */
        Status MultiplyTransform( Matrix* matrix, MatrixOrder order = MatrixOrderPrepend );
        /** Get the pen's transformation matrix */
        Status GetTransform( Matrix* matrix ) const;
        /** Set the pen's transformation matrix */
        Status SetTransform( const Matrix* matrix );
        /** Reset the pen's transformation matrix */
        Status ResetTransform(void);
    };

    /** @remarks
        // TODO: The following methods exist in GDI+, but not AggOO (yet):


                 PenAlignment GetAlignment(void);
                 Status SetAlignment( PenAlignment penAlignment );

                 Status GetCompoundArray( REAL* compoundArray, INT count );
                 INT GetCompoundArrayCount(void);
                 Status SetCompoundArray( const REAL* compoundArray, INT count );

                 Status GetCustomStartCap( CustomLineCap* customCap );
                 Status SetCustomStartCap( const CustomLineCap* customCap );
                 Status GetCustomEndCap( CustomLineCap* customCap );
                 Status SetCustomEndCap( const CustomLineCap* customCap );
                 Status GetDashPattern( REAL* dashArray, INT count );
                 Status SetDashPattern( const REAL* dashArray, INT count );
                 INT GetDashPatternCount(void);
    */

} // namespace AggOO

#endif  // AggOOPen_H_

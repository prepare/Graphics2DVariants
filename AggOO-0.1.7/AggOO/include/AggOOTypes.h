
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
// $Id: AggOOTypes.h,v 1.12 2009/03/13 23:00:29 dratek Exp $
//
// AggOOTypes.h -- Defines types for the AggOO library.
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

#ifndef AggOOTypes_H_
#define AggOOTypes_H_

#include "AggOOPlatform.h"

#include <limits>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
using std::min;
using std::max;

#include <math.h>

namespace AggOO
{
    typedef float   REAL;
    typedef int     INT;
    typedef unsigned int* PUINT;
//    typedef unsigned long ARGB;
    typedef std::string STRING;
    typedef std::wstring WSTRING;

    /**! Check REAL values for equivalence */
    inline bool RealEqual( REAL r1, REAL r2 )
    {
        return ( static_cast< REAL >( fabs( r1 - r2 ) ) <= std::numeric_limits< REAL >::epsilon() );
    }

/*
    template < class T >
    class Array : public std::vector< T >
    {
    public:
        inline friend std::ostream& operator<<( std::ostream& stream, const Array& src )
        {
            std::ostream::sentry opfx(stream);
            if ( !opfx )
                return stream;
            stream << "{ ";
            Array::const_iterator iter = src.begin();
            while ( iter != src.end() )
            {
                stream << *iter;
                iter++;
                if ( iter != src.end() )
                    stream << ", ";
            }
            stream << " }";
            return stream;
        }
    };
*/
//    typedef std::vector Array;

    //! Write to a stream
    template< typename T >
    inline std::ostream& operator<<( std::ostream& stream, const std::vector< T >& src )
    {
            std::ostream::sentry opfx(stream);
            if ( !opfx )
                return stream;
            stream << "[ ";
            std::vector<T>::const_iterator iter = src.begin();
            while ( iter != src.end() )
            {
                stream << *iter;
                iter++;
                if ( iter != src.end() )
                    stream << ", ";
            }
            stream << " ]";
            return stream;
    }

#ifndef BYTE
#   define BYTE unsigned char
#endif
#ifndef UINT
#   define UINT unsigned int
#endif
#ifndef UINT_PTR
#   define UINT_PTR unsigned int*
#endif

#ifndef CHAR
#   define CHAR char
#endif
#ifndef WCHAR
#   define WCHAR wchar_t
#endif

#ifndef VOID
#   define VOID void
#endif

#ifndef BOOL
#   define BOOL INT
#   define FALSE 0
#   define TRUE  1
#endif
#ifndef DWORD
#   define DWORD unsigned long
#endif
#ifndef ULONG
#   define ULONG unsigned long
#endif
#ifndef ULONG_PTR
#   define ULONG_PTR unsigned long*
#endif
#ifndef DWORDLONG
#   define DWORDLONG unsigned __int64
#endif

    typedef DWORD ARGB;
    typedef DWORDLONG ARGB64;

    typedef unsigned char   UInt8;
    typedef char            SInt8;
    typedef unsigned short  UInt16;
    typedef short           SInt16;
    typedef unsigned long   UInt32;
    typedef long            SInt32;

#if ( AGGOO_PLATFORM == AGGOO_PLATFORM_WIN32 )
    typedef unsigned __int64 UInt64;
    typedef __int64          SInt64;
#else
    typedef unsigned long long UInt64;
    typedef long long          SInt64;
#endif

#ifndef COLORREF
    typedef DWORD COLORREF;
#endif
#ifndef RGB
    inline COLORREF RGB( BYTE r, BYTE g, BYTE b ) { return (COLORREF)( (ARGB( r ) | ( ARGB( g ) << 8 ) | ( ARGB( b ) << 16 ) ) ); };
#endif
#ifndef GetRValue
    inline BYTE GetRValue( COLORREF rgb )   { return BYTE( rgb ); }
#endif
#ifndef GetGValue
    inline BYTE GetGValue( COLORREF rgb )   { return BYTE( rgb >> 8 ); }
#endif
#ifndef GetBValue
    inline BYTE GetBValue( COLORREF rgb )   { return BYTE( rgb >> 16 ); }
#endif

#ifndef LANGID
#   define LANGID unsigned long
#   define LANG_NEUTRAL     0x00
#   define SUBLANG_NEUTRAL  0x00
#endif

    //--------------------------------------------------------------------------
    // Callback functions
    //--------------------------------------------------------------------------

    extern "C" {

#ifndef CALLBACK
#define CALLBACK 
//#   ifdef _MAC
//#       define CALLBACK    PASCAL
//#   elif (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
//#       define CALLBACK    __stdcall
//#   else
//#       define CALLBACK
//#   endif
#endif

        typedef BOOL (CALLBACK * ImageAbort)(VOID *);
        typedef ImageAbort DrawImageAbort;
        typedef ImageAbort GetThumbnailImageAbort;
    }

    // Forward declare classes
    class Size;
    class SizeF;
    class Point;
    class PointExt;
    class PointF;
    class PointExtF;
    class Rect;
    class RectF;
    class PathData;
    class CharacterRange;

    //==========================================================================

    /** Status return values
    */
    enum Status
    {
        Ok = 0,
        GenericError = 1,
        InvalidParameter = 2,
        OutOfMemory = 3,
        ObjectBusy = 4,
        InsufficientBuffer = 5,
        NotImplemented = 6,
        Win32Error = 7,
        WrongState = 8,
        Aborted = 9,
        FileNotFound = 10,
        ValueOverflow = 11,
        AccessDenied = 12,
        UnknownImageFormat = 13,
        FontFamilyNotFound = 14,
        FontStyleNotFound = 15,
        NotTrueTypeFont = 16,
        UnsupportedGdiplusVersion = 17,
        GdiplusNotInitialized = 18,
        PropertyNotFound = 19,
        PropertyNotSupported = 20,

        InvalidLibraryVersion = 21
    };

    //==========================================================================

    /** Size class
        @remarks
            Contains 2D dimensions (Width and Height) as integers
    */
    class Size
    {
    public:
        INT     Width, Height;

    public:
        Size() : Width( 0 ), Height( 0 )   { }
        Size( const Size& sz ) : Width( sz.Width ), Height( sz.Height )   { }
        inline Size( const SizeF& sz );
        Size( INT width, INT height ) : Width( width ), Height( height )   { }

        /** Add a Size to this Size
        */
        Size operator+( const Size& sz )
        {
            Size temp;
            temp.Width  = Width + sz.Width;
            temp.Height = Height + sz.Height;
            return temp;
        }

        /** Subtract a Size from this Size
        */
        Size operator-( const Size& sz )
        {
            Size temp;
            temp.Width  = Width - sz.Width;
            temp.Height = Height - sz.Height;
            return temp;
        }

        /** Determine whether this Size is empty (width and height are 0)
        */
        BOOL Empty() const
        {
            return ( Width == 0 && Height == 0 );
        }

        /** Determine whether this Size equals another Size
        */
        BOOL Equals( const Size& sz ) const
        {
            return ( Width == sz.Width && Height == sz.Height );
        }
    };

    //! Write to a stream
    inline std::ostream& operator<<( std::ostream& stream, const Size& src )
    {
        std::ostream::sentry opfx(stream);
        if ( !opfx )
            return stream;
        return ( stream << "Size( " << src.Width << ", " << src.Height << " )" );
    }

    /** SizeF class
        @remarks
            Contains 2D dimensions (Width and Height) as floats
    */
    class SizeF
    {
    public:
        REAL    Width, Height;

    public:
        SizeF() : Width( 0.0 ), Height( 0.0 )   { }
        inline SizeF( const Size& sz );
        SizeF( const SizeF& sz ) : Width( sz.Width ), Height( sz.Height )   { }
        SizeF( REAL width, REAL height ) : Width( width ), Height( height )   { }

        /** Add a Size to this Size
        */
        SizeF operator+( const SizeF& sz )
        {
            SizeF temp;
            temp.Width  = Width + sz.Width;
            temp.Height = Height + sz.Height;
            return temp;
        }

        /** Subtract a Size from this Size
        */
        SizeF operator-( const SizeF& sz )
        {
            SizeF temp;
            temp.Width  = Width - sz.Width;
            temp.Height = Height - sz.Height;
            return temp;
        }

        /** Determine whether this Size is empty (width and height are 0)
        */
        BOOL Empty() const
        {
            return ( Width == 0 && Height == 0 );
        }

        /** Determine whether this Size equals another Size
        */
        BOOL Equals( const SizeF& sz ) const
        {
            return ( RealEqual( Width, sz.Width ) && RealEqual( Height, sz.Height ) );
        }
    };

    //! Write to a stream
    inline std::ostream& operator<<( std::ostream& stream, const SizeF& src )
    {
        std::ostream::sentry opfx(stream);
        if ( !opfx )
            return stream;
        return ( stream << "SizeF( " << src.Width << ", " << src.Height << " )" );
    }

    // Finish the size classes:
    Size::Size( const SizeF& sz ) : Width( sz.Width ), Height( sz.Height )    { }
    SizeF::SizeF( const Size& sz ) : Width( sz.Width ), Height( sz.Height )   { }

    //==========================================================================

    /** Point class
        @remarks
            Defines a 2D point X,Y as integers
    */
    class Point
    {
    public:
        INT     X, Y;

    public:
        Point() : X( 0 ), Y( 0 )    { }
        Point( INT x, INT y ) : X( x ), Y( y )  { }
        Point( const Size& sz ) : X( sz.Width ), Y( sz.Height )    { }
        Point( const Point& pt ) : X( pt.X ), Y( pt.Y )     { }
        inline Point( const PointF& pt );

        /** Negate the point */
        Point operator-() const
        {
            Point temp;
            temp.X = -X;
            temp.Y = -Y;
            return temp;
        }

        /** Add a Point to this Point
        */
        Point operator+( const Point& pt ) const
        {
            Point temp;
            temp.X = X + pt.X;
            temp.Y = Y + pt.Y;
            return temp;
        }
        Point operator+( REAL scalar ) const
        {
            Point temp;
            temp.X = X + scalar;
            temp.Y = Y + scalar;
            return temp;
        }

        /** Subtract a Point from this Point
        */
        Point operator-( const Point& pt ) const
        {
            Point temp;
            temp.X = X - pt.X;
            temp.Y = Y - pt.Y;
            return temp;
        }
        Point operator-( REAL scalar ) const
        {
            Point temp;
            temp.X = X - scalar;
            temp.Y = Y - scalar;
            return temp;
        }

        Point operator*( const Point& pt ) const
        {
            Point temp;
            temp.X = X * pt.X;
            temp.Y = Y * pt.Y;
            return temp;
        }
        Point operator*( REAL scalar ) const
        {
            Point temp;
            temp.X = X * scalar;
            temp.Y = Y * scalar;
            return temp;
        }
        Point operator/( const Point& pt ) const
        {
            Point temp( *this );
            if ( pt.X ) temp.X /= pt.X;
            if ( pt.Y ) temp.Y /= pt.Y;
            return temp;
        }
        Point operator/( REAL scalar ) const
        {
            Point temp( *this );
            if ( scalar )
            {
                temp.X /= scalar;
                temp.Y /= scalar;
            }
            return temp;
        }
        
        Point& operator+=( const Point& pt )
        {
            X += pt.X;
            Y += pt.Y;
            return *this;
        }
        Point& operator+=( REAL scalar )
        {
            X += scalar;
            Y += scalar;
            return *this;
        }
        Point& operator-=( const Point& pt )
        {
            X -= pt.X;
            Y -= pt.Y;
            return *this;
        }
        Point& operator-=( REAL scalar )
        {
            X -= scalar;
            Y -= scalar;
            return *this;
        }
        Point& operator*=( const Point& pt )
        {
            X *= pt.X;
            Y *= pt.Y;
            return *this;
        }
        Point& operator*=( REAL scalar )
        {
            X *= scalar;
            Y *= scalar;
            return *this;
        }
        Point& operator/=( const Point& pt )
        {
            if ( pt.X ) X /= pt.X;
            if ( pt.Y ) Y /= pt.Y;
            return *this;
        }
        Point& operator/=( REAL scalar )
        {
            if ( scalar )
            {
                X /= scalar;
                Y /= scalar;
            }
            return *this;
        }

        // Equality operators
        bool operator==( const Point& pt ) const
        {
            return ( X == pt.X && Y == pt.Y );
        }
        bool operator!=( const Point& pt ) const
        {
            return ( X != pt.X || Y != pt.Y );
        }

        // Comparison operators
        bool operator<( const Point& pt ) const
        {
            return ( X < pt.X && Y < pt.Y );
        }
        bool operator<=( const Point& pt ) const
        {
            return ( X <= pt.X && Y <= pt.Y );
        }
        bool operator>( const Point& pt ) const
        {
            return ( X > pt.X && Y > pt.Y );
        }
        bool operator>=( const Point& pt ) const
        {
            return ( X >= pt.X && Y >= pt.Y );
        }

        /** Determine whether this Point equals another Point
        */
        BOOL Equals( const Point& pt ) const
        {
            return ( X == pt.X && Y == pt.Y );
        }
    };

    //! Write to a stream
    inline std::ostream& operator<<( std::ostream& stream, const Point& src )
    {
        std::ostream::sentry opfx(stream);
        if ( !opfx )
            return stream;
        return ( stream << "Point( " << src.X << ", " << src.Y << " )" );
    }

    /** PointExt class
    */
    class PointExt : public Point
    {
    public:
        PointExt() : Point()    { }
        PointExt( INT x, INT y ) : Point( x, y )        { }
        PointExt( const Size& sz ) : Point( sz )        { }
        PointExt( const Point& pt ) : Point( pt )       { }
        PointExt( const PointF& pt ) : Point( pt )      { }

        //! Find the midpoint between two points
        static Point MidPoint( const Point& pt1, const Point& pt2 )
        {
            return Point(
                            ( pt2.X - pt1.X ) * 0.5 + pt1.X,
                            ( pt2.Y - pt1.Y ) * 0.5 + pt1.Y
                        );
        }

        //! Midpoint between this point and another point
        Point MidPoint( const Point& pt2 ) const
        {
            return MidPoint( *this, pt2 );
        }

        //! Interpolate between two points, finding a point on the line
        static Point Lerp( REAL ratio, const Point& pt1, const Point& pt2 )
        {
            return Point(
                            ( pt2.X - pt1.X ) * ratio + pt1.X,
                            ( pt2.Y - pt1.Y ) * ratio + pt1.Y
                        );
        }

        //! Interpolate between this point and another, finding a point on the
        //! line
        Point Lerp( REAL ratio, const Point& pt2 ) const
        {
            return Lerp( ratio, *this, pt2 );
        }

        //! Find the distance between two points
        static REAL Distance( const Point& pt1, const Point& pt2 )
        {
            return (INT)sqrt( REAL( ( pt2.X - pt1.X ) * ( pt2.X - pt1.X ) + \
                                    ( pt2.Y - pt1.Y ) * ( pt2.Y - pt1.Y ) ) );
        }

        //! Find the distance between this point and another
        REAL Distance( const Point& pt2 ) const
        {
            return Distance( *this, pt2 );
        }

        /** Find the length of a vector */
        static REAL Length( const Point& pt )
        {
            return (REAL)sqrt( REAL( pt.X * pt.X + pt.Y * pt.Y ) );
        }

        /** Find the length of this vector */
        REAL Length() const
        {
            return Length( *this );
        }

        /** Normalize the point/vector */
        static Point Normalize( const Point& pt )
        {
            REAL length = Distance( pt, Point( 0.0, 0.0 ) );
            if ( length )
                return Point( pt.X / length, pt.Y / length );
            return pt;
        }

        /** Normalize the point/vector */
        Point& Normalize()
        {
            *this = Normalize( *this );
            return *this;
        }

        /** Normalize the point/vector */
        Point NormalizeCopy() const
        {
            return Normalize( *this );
        }

        /** Set the length of a vector */
        Point SetLength( const Point& pt, REAL len )
        {
            return *this * ( len / Length() );
        }

        /** Set the length of this vector */
        Point SetLength( REAL len )
        {
            return SetLength( *this, len );
        }

        /** Get a perpendicular vector */
        static Point Perpendicular( const Point& pt )
        {
            return Point( -pt.Y, pt.X );
        }

        /** Get a perpendicular vector */
        Point Perpendicular() const
        {
            return Perpendicular( *this );
        }
    };

    /** PointF class
        @remarks
            Defines a 2D point X,Y as floats
    */
    class PointF
    {
    public:
        REAL     X, Y;

    public:
        PointF() : X( 0 ), Y( 0 )    { }
        PointF( REAL x, REAL y ) : X( x ), Y( y )  { }
        PointF( const Size& sz ) : X( sz.Width ), Y( sz.Height )    { }
        PointF( const PointF& pt ) : X( pt.X ), Y( pt.Y )     { }
        inline PointF( const Point& pt );

        /** Negate the point */
        PointF operator-() const
        {
            PointF temp;
            temp.X = -X;
            temp.Y = -Y;
            return temp;
        }

        /** Add a Point to this Point
        */
        PointF operator+( const PointF& pt ) const
        {
            PointF temp;
            temp.X = X + pt.X;
            temp.Y = Y + pt.Y;
            return temp;
        }
        PointF operator+( REAL scalar ) const
        {
            PointF temp;
            temp.X = X + scalar;
            temp.Y = Y + scalar;
            return temp;
        }

        /** Subtract a Point from this Point
        */
        PointF operator-( const PointF& pt ) const
        {
            PointF temp;
            temp.X = X - pt.X;
            temp.Y = Y - pt.Y;
            return temp;
        }
        PointF operator-( REAL scalar ) const
        {
            PointF temp;
            temp.X = X - scalar;
            temp.Y = Y - scalar;
            return temp;
        }

        PointF operator*( const PointF& pt ) const
        {
            PointF temp;
            temp.X = X * pt.X;
            temp.Y = Y * pt.Y;
            return temp;
        }
        PointF operator*( REAL scalar ) const
        {
            PointF temp;
            temp.X = X * scalar;
            temp.Y = Y * scalar;
            return temp;
        }
        PointF operator/( const PointF& pt ) const
        {
            PointF temp( *this );
            if ( pt.X ) temp.X /= pt.X;
            if ( pt.Y ) temp.Y /= pt.Y;
            return temp;
        }
        PointF operator/( REAL scalar ) const
        {
            PointF temp( *this );
            if ( scalar )
            {
                temp.X /= scalar;
                temp.Y /= scalar;
            }
            return temp;
        }
        
        PointF& operator+=( const PointF& pt )
        {
            X += pt.X;
            Y += pt.Y;
            return *this;
        }
        PointF& operator+=( REAL scalar )
        {
            X += scalar;
            Y += scalar;
            return *this;
        }
        PointF& operator-=( const PointF& pt )
        {
            X -= pt.X;
            Y -= pt.Y;
            return *this;
        }
        PointF& operator-=( REAL scalar )
        {
            X -= scalar;
            Y -= scalar;
            return *this;
        }
        PointF& operator*=( const PointF& pt )
        {
            X *= pt.X;
            Y *= pt.Y;
            return *this;
        }
        PointF& operator*=( REAL scalar )
        {
            X *= scalar;
            Y *= scalar;
            return *this;
        }
        PointF& operator/=( const PointF& pt )
        {
            if ( pt.X ) X /= pt.X;
            if ( pt.Y ) Y /= pt.Y;
            return *this;
        }
        PointF& operator/=( REAL scalar )
        {
            if ( scalar )
            {
                X /= scalar;
                Y /= scalar;
            }
            return *this;
        }

        // Equality operators
        bool operator==( const PointF& pt ) const
        {
            return Equals( pt );
        }
        bool operator!=( const PointF& pt ) const
        {
            return !Equals( pt );
        }

        // Comparison operators
        bool operator<( const Point& pt ) const
        {
            return ( X < pt.X && Y < pt.Y );
        }
        bool operator<=( const Point& pt ) const
        {
            return ( X <= pt.X && Y <= pt.Y );
        }
        bool operator>( const Point& pt ) const
        {
            return ( X > pt.X && Y > pt.Y );
        }
        bool operator>=( const Point& pt ) const
        {
            return ( X >= pt.X && Y >= pt.Y );
        }

        /** Determine whether this Point equals another Point
        */
        BOOL Equals( const PointF& pt ) const
        {
            return ( RealEqual( X, pt.X ) && RealEqual( Y, pt.Y ) );
        }
    };

    //! Write to a stream
    inline std::ostream& operator<<( std::ostream& stream, const PointF& src )
    {
        std::ostream::sentry opfx(stream);
        if ( !opfx )
            return stream;
        return ( stream << "PointF( " << src.X << ", " << src.Y << " )" );
    }

    /** PointExtF class
    */
    class PointExtF : public PointF
    {
    public:
        PointExtF() : PointF()    { }
        PointExtF( REAL x, REAL y ) : PointF( x, y )      { }
        PointExtF( const Size& sz ) : PointF( sz )      { }
        PointExtF( const PointF& pt ) : PointF( pt )    { }
        PointExtF( const Point& pt ) : PointF( pt )     { }

        /** Find the midpoint between two points */
        static PointF MidPoint( const PointF& pt1, const PointF& pt2 )
        {
            return PointF(
                            ( pt2.X - pt1.X ) * 0.5 + pt1.X,
                            ( pt2.Y - pt1.Y ) * 0.5 + pt1.Y
                        );
        }

        /** Midpoint between this point and another point */
        PointF MidPoint( const PointF& pt2 ) const
        {
            return MidPoint( *this, pt2 );
        }

        /** Interpolate between two points, finding a point on the line */
        static PointF Lerp( REAL ratio, const PointF& pt1, const PointF& pt2 )
        {
            return PointF(
                            ( pt2.X - pt1.X ) * ratio + pt1.X,
                            ( pt2.Y - pt1.Y ) * ratio + pt1.Y
                        );
        }

        /** Interpolate between this point and another, finding a point on the
            line
        */
        PointF Lerp( REAL ratio, const PointF& pt2 ) const
        {
            return Lerp( ratio, *this, pt2 );
        }

        /** Find the distance between two points */
        static REAL Distance( const PointF& pt1, const PointF& pt2 )
        {
            return (REAL)sqrt( REAL( ( pt2.X - pt1.X ) * ( pt2.X - pt1.X ) + \
                                     ( pt2.Y - pt1.Y ) * ( pt2.Y - pt1.Y ) ) );
        }

        /** Find the distance between this point and another */
        REAL Distance( const PointF& pt2 ) const
        {
            return Distance( *this, pt2 );
        }

        /** Find the length of a point */
        static REAL Length( const PointF& pt )
        {
            return (REAL)sqrt( REAL( pt.X * pt.X + pt.Y * pt.Y ) );
        }

        /** Find the length of this point */
        REAL Length() const
        {
            return Length( *this );
        }

        /** Normalize the point/vector */
        static PointF Normalize( const PointF& pt )
        {
            REAL length = Distance( pt, PointF( 0.0, 0.0 ) );
            if ( length )
                return PointF( pt.X / length, pt.Y / length );
            return pt;
        }

        /** Normalize the point/vector */
        PointF Normalize()
        {
            *this = Normalize( *this );
            return *this;
        }

        /** Normalize the point/vector */
        PointF NormalizeCopy() const
        {
            return Normalize( *this );
        }

        /** Set a vector to a given length */
        static PointF SetLength( const PointF& pt, REAL len )
        {
            return pt * ( len / Length( pt ) );
        }

        /** Set this vector to a given length */
        PointF SetLength( REAL len )
        {
            return SetLength( *this, len );
        }

        /** Get a perpendicular vector */
        static PointF Perpendicular( const PointF& pt )
        {
            return PointF( -pt.Y, pt.X );
        }

        /** Get a perpendicular vector */
        PointF Perpendicular() const
        {
            return Perpendicular( *this );
        }

        /** Calculate the dot product of two points */
        static REAL Dot( const PointF& pt1, const PointF& pt2 )
        {
            return pt1.X * pt2.X + pt1.Y * pt2.Y;
        }

        /** Calculate the dot product of two points */
        REAL Dot( const PointF& pt2 )
        {
            return X * pt2.X + Y * pt2.Y;
        }

        /** Find a point on a line that is closest to an arbitrary point (this
            point and the arbitrary point will be perpendicular to the line.)
            @param  endPt1          First end point of the line
            @param  endPt2          Second end point of the line
            @param  pt              Arbitrary point
            @param  clampToSegment  Indicates if the point should be clamped to
                                    the end points of the path.
        */
        static PointF ClosestPointOnLine( const PointF& endPt1, const PointF& endPt2, const PointF& pt, bool clampToSegment = false )
        {
            // 'ptLinePt1' and 'ptLinePt2' define the line segment.  We find the point on this
            // segment that is closest to 'this' point and return that point.

            // First, we need a vector from the first point to 'this' point:
            PointF vec1( pt - endPt1 );

            // We want the normalized vector from the second end-point to the first:
            PointF vec2( endPt2 - endPt1 );
            vec2 = PointExtF::Normalize( vec2 );

            // Find the distance between the end-points (ie. find the length of the line segment):
            REAL      dist   = PointExtF::Distance( endPt1, endPt2 );

            // Take the dot product of vec1 and vec2.
            REAL      t   = PointExtF::Dot( vec2, vec1 );

            // If the point is to be clamped to the segment, then if t is less
            // than 0, the closest point is endPt1.  If t is greater than the
            // line length, the closest point is endPt2.
            if ( clampToSegment )
            {
                if ( t <= 0 )
                {
                    // linePt1 is closest.  Either 'this' point is directly perpendicular, or
                    // we are out of range here:
                    return endPt1;
                }

                if ( t >= dist )
                {
                    // We intersect either right on, or past the second point:
                    return endPt2;
                }
            }

            // The point is somewhere between the two end-points.  To find it, create
            // a vector in the direction of the line segment, with a length of t (which
            // is the distance from the first point to the 'closest' point:
            PointF vec3( vec2 * t );

            // The closest point will be vec3 + ptLinePt1:
            return vec3 + endPt1;
        }

        /** Determine which side of a line that a given point is on.
            @param  endPt1          First point on the line
            @param  endPt2          Second point on the line
            @param  pt              Arbitrary point
            @return 1 if the point is "above" the line, -1 if the point is below
                    the line, and 0 if the point is on the line.
        */
        static INT SideOfLine( const PointF& endPt1, const PointF& endPt2, const PointF& pt )
        {
            // Use the general formula Ax + By + C = 0.  Find A, B, and C (note
            // that the vector perpendicular to the line is (A,B), and C=-(Ax+By).
            PointF lineVec = endPt2 - endPt1;
            PointF lineNorm( -lineVec.Y, lineVec.X );
            REAL A = lineNorm.X, B = lineNorm.Y, C = -( A * endPt1.X + B * endPt1.Y );

            // Plug the arbitrary point into the function:
            REAL testVal = A * pt.X + B * pt.Y + C;
            if ( testVal < 0 )
                return -1;
            if ( testVal )
                return 1;
            return 0;
        }
    };

    // Finish the point classes:
    Point::Point( const PointF& pt ) : X( pt.X ), Y( pt.Y ) { }
    PointF::PointF( const Point& pt ) : X( pt.X ), Y( pt.Y ) { }

    //==========================================================================

    /** Rect class
        @remarks
            Represents a rectangle as integers
    */
    class Rect
    {
    public:
        INT     X, Y, Width, Height;

    public:
        Rect() : X( 0 ), Y( 0 ), Width( 0 ), Height( 0 )    { }
        Rect( INT x, INT y, INT w, INT h ) : X( x ), Y( y ), Width( w ), Height( h )
        {
            Normalize();
        }
        Rect( const Point& pt, const Size& sz ) : X( pt.X ), Y( pt.Y ), Width( sz.Width ), Height( sz.Height )
        {
            Normalize();
        }
        Rect( const Rect& rc ) : X( rc.X ), Y( rc.Y ), Width( rc.Width ), Height( rc.Height ) { }
        inline Rect( const RectF& rc );

        /** Clone this rect
        */
        Rect* Clone() const
        {
            return new Rect( X, Y, Width, Height );
        }

        /** Normalize the rect.  If the width or height is negative, invert the
            dimension.
        */
        void Normalize()
        {
            if ( Width < 0 )
            {
                X += Width;
                Width = -Width;
            }
            if ( Height < 0 )
            {
                Y += Height;
                Height = -Height;
            }
        }

        /** Get the location of the rect
            @remarks
                The location is the top-left corner
        */
        VOID GetLocation( Point* pt ) const
        {
            pt->X = X;
            pt->Y = Y;
        }

        /** Get the size of the rect
        */
        VOID GetSize( Size* sz ) const
        {
            sz->Width = Width;
            sz->Height = Height;
        }

        /** Get the boundary of the rect
        */
        VOID GetBounds( Rect* rc ) const
        {
            rc->X       = X;
            rc->Y       = Y;
            rc->Width   = Width;
            rc->Height  = Height;
        }

        INT GetLeft() const     { return X; }
        INT GetTop() const      { return Y; }
        INT GetRight() const    { return X + Width; }
        INT GetBottom() const   { return Y + Height; }

        /** Check whether the rect is empty (size = 0,0)
        */
        BOOL IsEmptyArea() const
        {
            return ( Width <= 0 || Height <= 0 );
        }

        /** Check whether the rect equals another rect
        */
        BOOL Equals( const Rect& rc ) const
        {
            return ( X == rc.X &&
                     Y == rc.Y &&
                     Width == rc.Width &&
                     Height == rc.Height );
        }

        /** Check whether a point is inside the rect
        */
        BOOL Contains( INT x, INT y ) const
        {
            return ( x >= X && x < X + Width && y >= Y && y < Y + Height );
        }
        BOOL Contains( const Point& pt ) const
        {
            return ( pt.X >= X && pt.X < X + Width && pt.Y >= Y && pt.Y < Y + Height );
        }

        /** Check whether another rect is inside this rect
        */
        BOOL Contains( const Rect& rc ) const
        {
            return ( ( X <= rc.X ) && ( rc.GetRight() <= GetRight() ) &&
                     ( Y <= rc.Y ) && ( rc.GetBottom() <= GetBottom() ) );
        }

        /** Grow the rect
        */
        VOID Inflate( INT dx, INT dy )
        {
            X -= dx;
            Y -= dy;
            Width += 2 * dx;
            Height += 2 * dy;
        }
        VOID Inflate( const Point& pt )
        {
            Inflate( pt.X, pt.Y );
        }

        /** Intersect this rect with another rect
            @remarks
                This rect will result in the area contained by both rects.
            @return
                True if the intersected area is not empty
        */
        BOOL Intersect( const Rect& rc )
        {
            Intersect( *this, *this, rc );
        }
        BOOL Intersect( Rect& out, const Rect& a, const Rect& b )
        {
            INT right   = min( a.GetRight(), b.GetRight() );
            INT bottom  = min( a.GetBottom(), b.GetBottom() );
            INT left    = max( a.GetLeft(), b.GetLeft() );
            INT top     = max( a.GetTop(), b.GetTop() );

            out.X       = left;
            out.Y       = top;
            out.Width   = right - left;
            out.Height  = bottom - top;

            return !out.IsEmptyArea();
        }

        /** Check whether this rect intersects with another
            @remarks
                Does not change the value of either rect
        */
        BOOL IntersectsWith( const Rect& rc ) const
        {
            return ( ( GetLeft() < rc.GetRight() ) &&
                     ( GetTop() < rc.GetBottom() ) &&
                     ( GetRight() > rc.GetLeft() ) &&
                     ( GetBottom() > rc.GetTop() ) );
        }

        /** Create a rect that is the union of this and another rect
            @return
                True if the union is not empty
        */
        BOOL Union( Rect& out, const Rect& a, const Rect& b )
        {
            INT right   = max( a.GetRight(), b.GetRight() );
            INT bottom  = max( a.GetBottom(), b.GetBottom() );
            INT left    = min( a.GetLeft(), b.GetLeft() );
            INT top     = min( a.GetTop(), b.GetTop() );

            out.X       = left;
            out.Y       = top;
            out.Width   = right - left;
            out.Height  = bottom - top;

            return !out.IsEmptyArea();
        }

        /** Offset the rect
        */
        VOID Offset( INT dx, INT dy )
        {
            X += dx;
            Y += dy;
        }
        VOID Offset( const Point& pt )
        {
            Offset( pt.X, pt.Y );
        }
    };

    //! Write to a stream
    inline std::ostream& operator<<( std::ostream& stream, const Rect& src )
    {
        std::ostream::sentry opfx(stream);
        if ( !opfx )
            return stream;
        return ( stream << "Rect( " << src.X << ", " << src.Y << ", " << src.Width << ", " << src.Height << " )" );
    }

    /** RectF class
        @remarks
            Represents a rectangle as integers
    */
    class RectF
    {
    public:
        REAL     X, Y, Width, Height;

    public:
        RectF() : X( 0 ), Y( 0 ), Width( 0 ), Height( 0 )    { }
        RectF( REAL x, REAL y, REAL w, REAL h ) : X( x ), Y( y ), Width( w ), Height( h )
        {
            Normalize();
        }
        RectF( const PointF& pt, const SizeF& sz ) : X( pt.X ), Y( pt.Y ), Width( sz.Width ), Height( sz.Height )
        {
            Normalize();
        }
        RectF( const RectF& rc ) : X( rc.X ), Y( rc.Y ), Width( rc.Width ), Height( rc.Height ) { }
        inline RectF( const Rect& rc );

        /** Clone this RectF
        */
        RectF* Clone() const
        {
            return new RectF( X, Y, Width, Height );
        }

        /** Normalize the rect.  If the width or height is negative, invert the
            dimension.
        */
        void Normalize()
        {
            if ( Width < 0 )
            {
                X += Width;
                Width = -Width;
            }
            if ( Height < 0 )
            {
                Y += Height;
                Height = -Height;
            }
        }

        /** Get the location of the RectF
            @remarks
                The location is the top-left corner
        */
        VOID GetLocation( PointF* pt ) const
        {
            pt->X = X;
            pt->Y = Y;
        }

        /** Get the SizeF of the RectF
        */
        VOID GetSize( SizeF* sz ) const
        {
            sz->Width = Width;
            sz->Height = Height;
        }

        /** Get the boundary of the RectF
        */
        VOID GetBounds( RectF* rc ) const
        {
            rc->X       = X;
            rc->Y       = Y;
            rc->Width   = Width;
            rc->Height  = Height;
        }

        REAL GetLeft() const     { return X; }
        REAL GetTop() const      { return Y; }
        REAL GetRight() const    { return X + Width; }
        REAL GetBottom() const   { return Y + Height; }

        /** Check whether the RectF is empty (SizeF = 0,0)
        */
        BOOL IsEmptyArea() const
        {
            return ( Width <= 0 || Height <= 0 );
        }

        /** Check whether the RectF equals another RectF
        */
        BOOL Equals( const RectF& rc ) const
        {
            return ( RealEqual( X, rc.X ) &&
                     RealEqual( Y, rc.Y ) &&
                     RealEqual( Width, rc.Width ) &&
                     RealEqual( Height, rc.Height ) );
        }

        /** Check whether a PointF is inside the RectF
        */
        BOOL Contains( REAL x, REAL y ) const
        {
            return ( x >= X && x < X + Width && y >= Y && y < Y + Height );
        }
        BOOL Contains( const PointF& pt ) const
        {
            return ( pt.X >= X && pt.X < X + Width && pt.Y >= Y && pt.Y < Y + Height );
        }

        /** Check whether another RectF is inside this RectF
        */
        BOOL Contains( const RectF& rc ) const
        {
            return ( ( X <= rc.X ) && ( rc.GetRight() <= GetRight() ) &&
                     ( Y <= rc.Y ) && ( rc.GetBottom() <= GetBottom() ) );
        }

        /** Grow the RectF
        */
        VOID Inflate( REAL dx, REAL dy )
        {
            X -= dx;
            Y -= dy;
            Width += 2 * dx;
            Height += 2 * dy;
        }
        VOID Inflate( const PointF& pt )
        {
            Inflate( pt.X, pt.Y );
        }

        /** Intersect this RectF with another RectF
            @remarks
                This RectF will result in the area contained by both rects.
            @return
                True if the intersected area is not empty
        */
        BOOL Intersect( const RectF& rc )
        {
            Intersect( *this, *this, rc );
        }
        BOOL Intersect( RectF& out, const RectF& a, const RectF& b )
        {
            REAL right   = min( a.GetRight(), b.GetRight() );
            REAL bottom  = min( a.GetBottom(), b.GetBottom() );
            REAL left    = max( a.GetLeft(), b.GetLeft() );
            REAL top     = max( a.GetTop(), b.GetTop() );

            out.X       = left;
            out.Y       = top;
            out.Width   = right - left;
            out.Height  = bottom - top;

            return !out.IsEmptyArea();
        }

        /** Check whether this RectF intersects with another
            @remarks
                Does not change the value of either RectF
        */
        BOOL IntersectsWith( const RectF& rc ) const
        {
            return ( ( GetLeft() < rc.GetRight() ) &&
                     ( GetTop() < rc.GetBottom() ) &&
                     ( GetRight() > rc.GetLeft() ) &&
                     ( GetBottom() > rc.GetTop() ) );
        }

        /** Create a RectF that is the union of this and another RectF
            @return
                True if the union is not empty
        */
        BOOL Union( RectF& out, const RectF& a, const RectF& b )
        {
            REAL right   = max( a.GetRight(), b.GetRight() );
            REAL bottom  = max( a.GetBottom(), b.GetBottom() );
            REAL left    = min( a.GetLeft(), b.GetLeft() );
            REAL top     = min( a.GetTop(), b.GetTop() );

            out.X       = left;
            out.Y       = top;
            out.Width   = right - left;
            out.Height  = bottom - top;

            return !out.IsEmptyArea();
        }

        /** Offset the RectF
        */
        VOID Offset( REAL dx, REAL dy )
        {
            X += dx;
            Y += dy;
        }
        VOID Offset( const PointF& pt )
        {
            Offset( pt.X, pt.Y );
        }
    };

    //! Write to a stream
    inline std::ostream& operator<<( std::ostream& stream, const RectF& src )
    {
        std::ostream::sentry opfx(stream);
        if ( !opfx )
            return stream;
        return ( stream << "RectF( " << src.X << ", " << src.Y << ", " << src.Width << ", " << src.Height << " )" );
    }

    // Finish the rect classes:
    Rect::Rect( const RectF& rc ) : X( rc.X ), Y( rc.Y ), Width( rc.Width ), Height( rc.Height ) { }
    RectF::RectF( const Rect& rc ) : X( rc.X ), Y( rc.Y ), Width( rc.Width ), Height( rc.Height ) { }

    //==========================================================================

    /** PathData class
        @remarks
            Defines a path
    */
    class PathData
    {
    public:
        INT     Count;                      // Number of vertices
        PointF* Points;                     // Path vertices
        BYTE*   Types;

    private:
        // Prevent accidental copying
        PathData( const PathData& path );
        PathData& operator=( const PathData& path );

    public:
        PathData() : Count( 0 ), Points( 0 ), Types( 0 )    { }

        ~PathData()
        {
            if ( Points )
                delete [] Points;
            if ( Types )
                delete [] Types;
        }
    };

    //! Write to a stream
    inline std::ostream& operator<<( std::ostream& stream, const PathData& src )
    {
        std::ostream::sentry opfx(stream);
        if ( !opfx )
            return stream;
        stream << "PathData( \n";
        stream << "          Count = " << src.Count << ", \n";
        for ( INT i = 0; i < src.Count; i++ )
        {
            stream << "          Vertex " << i << " = " << src.Points[ i ];
            if ( i + 1 < src.Count )
                stream << ", \n";
        }
        stream << ")\n";

        return stream;
    }

    //==========================================================================

    /** CharacterRange class
    */
    class CharacterRange
    {
    public:
        INT     First;                      // First character in the range
        INT     Last;                       // Last character in the range

    public:
        CharacterRange() : First( 0 ), Last( 0 )    { }
        CharacterRange( INT first, INT last ) : First( first ), Last( last )    { }

        CharacterRange& operator=( const CharacterRange& cr )
        {
            First = cr.First;
            Last  = cr.Last;
            return *this;
        }
    };

    //! Write to a stream
    inline std::ostream& operator<<( std::ostream& stream, const CharacterRange& src )
    {
        std::ostream::sentry opfx(stream);
        if ( !opfx )
            return stream;
        return ( stream << "CharacterRange( " << src.First << ", " << src.Last << " )" );
    }

} // namespace AggOO

#endif  // AggOOTypes_H_

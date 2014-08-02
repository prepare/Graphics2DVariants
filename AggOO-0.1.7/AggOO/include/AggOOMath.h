
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
// $Id: AggOOMath.h,v 1.7 2008/08/13 22:08:29 dratek Exp $
//
// AggOOMath.h -- Collection of math routines.
//
// Author       : Chad M. Draper
// Date         : October 18, 2006
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2006-10-18      Initial implementation
//

#ifndef AggOOMath_H_
#define AggOOMath_H_

#include <limits>
#include <math.h>
#include <agg_math.h>
#include "AggOOTypes.h"

namespace AggOO
{
    const REAL pi           = 4.0 * atan( 1.0 );    /**< Constant value of PI */
    const REAL two_pi       = 2.0 * pi;             /**< Constant value of 2*PI */
    const REAL half_pi      = 0.5 * pi;             /**< Constant value of PI / 2.0 */

    /** Get the sign of a number (+/- 1) */
    inline INT ISign( INT num )     { return ( num < 0 ) ? -1 : 1; }
    /** Get the sign of a number (+/- 1) */
    inline REAL Sign( REAL num )    { return ( num < 0.0 ) ? -1 : 1; }

    /** Return the absolute value of a number */
    inline INT IAbs( INT num )      { return ( num < 0 ) ? -num : num; }
    /** Return the absolute value of a number */
    inline REAL Abs( REAL num )     { return ( num < 0.0 ) ? -num : num; }

    /** Return the ceiling of a value */
    inline INT ICeil( REAL num )    { return INT( ceil( num ) ); }
    /** Return the ceiling of a value */
    inline REAL Ceil( REAL num )    { return REAL( ceil( num ) ); }

    /** Return the floor of a value */
    inline INT IFloor( REAL num )   { return INT( floor( num ) ); }
    /** Return the floor of a value */
    inline REAL Floor( REAL num )   { return REAL( floor( num ) ); }

    /** Round a value to the nearest whole number */
    inline REAL IRound( REAL num )  { return IFloor( num + 0.5 ); }
    /** Round a value to the nearest whole number */
    inline REAL Round( REAL num )   { return Floor( num + 0.5 ); }

    /** Swap two values */
    inline void ISwap( INT& v1, INT& v2 )  { v1 ^= v2; v2 ^= v1; v1 ^= v2; }
    /** Swap two values */
    inline void Swap( REAL& v1, REAL& v2 )
    {
        REAL temp = v1;
        v1 = v2;
        v2 = temp;
    }

    /** Generate a random value between [0..1] */
    inline REAL UnitRandom()                        { return rand() / REAL( RAND_MAX ); };
    /** Generate a random value within a range */
    inline INT IRangeRandom( INT low, INT high )    { return IRound( UnitRandom() * ( high - low ) + low ); }
    /** Generate a random value within a range */
    inline REAL RangeRandom( REAL low, REAL high )  { return ( UnitRandom() * ( high - low ) + low ); }
    /** Generate a random value in the range [-val..val] */
    inline INT ISymmetricRandom( INT val )          { return IRound( UnitRandom() * 2.0 * val - val ); }
    /** Generate a random value in the range [-val..val] */
    inline REAL SymmetricRandom( REAL val )         { return ( UnitRandom() * 2.0 * val - val ); }

    /** Clamp a value to a range */
    inline INT IClamp( INT val, INT low, INT high ) { return ( ( val < low ) ? low : ( val > high ) ? high : val ); }
    /** Clamp a value to a range */
    inline REAL Clamp( REAL val, REAL low, REAL high ) { return ( ( val < low ) ? low : ( val > high ) ? high : val ); }
    /** Clamp a value to a minimum */
    inline INT IMinClamp( INT val, INT low )        { return ( val < low ) ? low : val; }
    /** Clamp a value to a minimum */
    inline REAL MinClamp( REAL val, REAL low )      { return ( val < low ) ? low : val; }
    /** Clamp a value to a maximum */
    inline INT IMaxClamp( INT val, INT high )       { return ( val > high ) ? high : val; }
    /** Clamp a value to a maximum */
    inline REAL MaxClamp( REAL val, REAL high )     { return ( val > high ) ? high : val; }

    /** Square root */
    inline INT ISqrt( INT val )                     { return (INT)sqrt( (double)val ); }
    /** Square root */
    inline REAL Sqrt( REAL val )                    { return (REAL)sqrt( val ); }
    /** Square of a value */
    inline INT ISqr( INT val )                      { return val * val; }
    /** Square of a value */
    inline REAL Sqr( REAL val )                     { return val * val; }

    /** Convert a value from degrees to radians */
    inline REAL Deg2Rad( REAL val )                 { return agg::deg2rad( val ); }
    /** Convert a value from radians to degrees */
    inline REAL Rad2Deg( REAL val )                 { return agg::rad2deg( val ); }

    /** Linearly interpolate two values based on a ratio */
    inline INT ILerp( REAL ratio, INT low, INT high )
    {
        INT diff = high - low;
        return INT( low + diff * ratio );
    }
    /** Linearly interpolate two values based on a ratio */
    inline REAL Lerp( REAL ratio, REAL low, REAL high )
    {
        REAL diff = high - low;
        return low + diff * ratio;
    }

    /** Calculate the modulus of a value and range */
    inline INT IModRange( INT val, INT low, INT high )
    {
        if ( low == high )
            return low;
        if ( high < low )
            ISwap( low, high );

        INT range = high - low;
        val -= low;
        if ( val < 0 )
        {
            val %= range;
            val += range;
        }
        else
            val %= range;

        val += low;

        return val;
    }
    /** Calculate the modulus of a value and range (inclusive) */
    inline INT IModRangeInclusive( INT val, INT low, INT high )
    {
        if ( low == high )
            return low;

        return IModRange( val, low, high + 1 );
    }

    /** Calculate the modulus of a value and range */
    inline REAL ModRange( REAL val, REAL low, REAL high )
    {
        if ( low == high )
            return low;
        if ( high < low )
            Swap( low, high );

        REAL range = high - low;
        val -= low;
        if ( val < 0 )
        {
            val = fmod( val, range );
            val = fmod( val + range, range );
        }
        else
            val = fmod( val, range );

        val += low;

        return val;
    }
    /** Calculate the modulus of a value and range (inclusive) */
    inline REAL ModRangeInclusive( REAL val, REAL low, REAL high )
    {
        if ( low == high )
            return low;

        if ( high < low )
            Swap( low, high );
        REAL range = high - low;
        while ( val < low )     val += range;
        while ( val > high )    val -= range;

        return val;
    }

    /**
        Reflect a value about a specfied pivot.
        @remarks
        The value is mirrored about the pivot so that the new value equals
        the pivot plus the pivot minus the original value.  There is no
        additional bounds checking.  For example, reflect( 3, 4 ) = 5,
        reflect( 10, 3 ) = -4.
    */
    static inline int IReflect( int val, int pivot )        { return pivot + ( pivot - val ); }
    static inline REAL Reflect( REAL val, REAL pivot )      { return pivot + ( pivot - val ); }

    /**
        Keep a value within a range, by reflecting it about the boundaries.
        @remarks
        If the value is less than the minimum of the range, reflect about
        the minimum.  If the value is greater than the range maximum,
        reflect about the maximum.  If the value is within the range, do
        nothing.
    */
    static inline int IReflectRange( int val, int minVal, int maxVal )
    {
        if ( maxVal < minVal )
            ISwap( minVal, maxVal );
        int diff = ( maxVal - minVal );
        val = IAbs( ( val - minVal ) % ( diff * 2 ) );
        if ( val > diff )
            val = IReflect( val, diff );
        val += minVal;

        return val;
    }
    static inline REAL ReflectRange( REAL val, REAL minVal, REAL maxVal )
    {
        if ( maxVal < minVal )
            Swap( minVal, maxVal );
        REAL diff = ( maxVal - minVal );
        val = Abs( fmod( val - minVal, diff * REAL( 2.0 ) ) );
        if ( val > diff )
            val = Reflect( val, diff );
        val += minVal;

        return val;
    }

    /**
        Reflect a value the boundaries of a range, using a sine curve.  This
        will result in a smoother reflection than ReflectRange.
    */
    static inline REAL ReflectSine( REAL val, REAL minVal, REAL maxVal )
    {
        // Calculate the ratio:
        REAL diff = maxVal - minVal;
        REAL ratio = ( val - minVal ) / ( 2.0 * diff );

        // Interpolate using a sine curve:
        return ( sin( ratio * two_pi - half_pi ) * 0.5 + 0.5 ) * diff + minVal;
    }

    /**
        Convert rectangular coordinates to polar coordinates
        @param ang          [out] Angle from the center to the point
        @param dist         [out] Distance from the center to the point
        @param x            [in]  X-coordinate of the point
        @param y            [in]  Y-coordinate of the point
        @param centerX      [in]  X-coordinate of the center
        @param centerY      [in]  Y-coordinate of the center
        @param scaleX       [in]  Horizontal scaling
        @param scaleY       [in]  Vertical scaling
    */
    inline void RectToPolar( REAL* ang, REAL* dist, REAL x, REAL y, REAL centerX, REAL centerY, REAL scaleX = 1.0, REAL scaleY = 1.0 )
    {
        // Calculate the offsets:
        REAL dx = ( x - centerX ) * scaleX;
        REAL dy = ( y - centerY ) * scaleY;

        // The angle is calculated as the inverse tangent of dy/dx.  If
        // dx == 0, then the angle is either +/- 90 degrees, based on the
        // sign of dy.
        if ( dx == 0.0 )
        {
            *ang = half_pi * Sign( dy );
        }
        else
            *ang = atan2( dy, dx );
        *dist = sqrt( dx * dx + dy * dy );
    }

    /**
        Convert polar coordinates to rectangular coordinates
        @param ang          [in]  Angle from the center to the point
        @param dist         [in]  Distance from the center to the point
        @param x            [out] X-coordinate of the point
        @param y            [out] Y-coordinate of the point
        @param centerX      [in]  X-coordinate of the center
        @param centerY      [in]  Y-coordinate of the center
    */
    inline void PolarToRect( REAL ang, REAL dist, REAL* x, REAL* y, REAL centerX, REAL centerY )
    {
        *x = centerX + dist * cos( ang );
        *y = centerY + dist * sin( ang );
    }
}

#endif  // AggOOMath_H_

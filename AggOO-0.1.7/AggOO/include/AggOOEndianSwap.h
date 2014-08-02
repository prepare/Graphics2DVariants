
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


/*! $Id: AggOOEndianSwap.h,v 1.2 2008/08/13 22:08:28 dratek Exp $
 *  @file   AggOOEndianSwap.h
 *  @author Chad M. Draper
 *  @date   April 25, 2008
 *  @brief  Methods to swap bytes based on the endianness of the source and
 *          of the current platform.
 *  
 */

#ifndef AggOOEndianSwap_H_
#define AggOOEndianSwap_H_

#include "AggOOPlatform.h"
#include "AggOOPrereqs.h"
#include "AggOOTypes.h"

namespace AggOO
{
    /** @class EndianSwap
        Class with static methods for rearranging bytes based on the endianness
        of the source and the system.
    */
    class EndianSwap
    {
    public:
        /** Swap the bytes of a 16 bit value */
        static UInt16 Swap16( UInt16 src )
        {
            return ( ( src & 0x00ff ) << 8 ) | ( ( src & 0xff00 ) >> 8 );
        }

        /** Swap the bytes of a 32 bit value */
        static UInt32 Swap32( UInt32 src )
        {
            return ( ( src & 0x000000ff ) << 24 ) |
                   ( ( src & 0x0000ff00 ) << 8  ) |
                   ( ( src & 0x00ff0000 ) >> 8  ) |
                   ( ( src & 0xff000000 ) >> 24 );
        }

        /** Swap the bytes of a 64 bit value */
        static UInt64 Swap64( UInt64 src )
        {
#if ( AGGOO_COMPILER == AGGOO_COMPILER_GNUC )
            return ( ( src & 0x00000000000000ffLL ) << 56 ) |
                   ( ( src & 0x000000000000ff00LL ) << 40 ) |
                   ( ( src & 0x0000000000ff0000LL ) << 24 ) |
                   ( ( src & 0x00000000ff000000LL ) <<  8 ) |
                   ( ( src & 0x000000ff00000000LL ) >>  8 ) |
                   ( ( src & 0x0000ff0000000000LL ) >> 24 ) |
                   ( ( src & 0x00ff000000000000LL ) >> 40 ) |
                   ( ( src & 0xff00000000000000LL ) >> 56 );
#else
            return ( ( src & 0x00000000000000ff ) << 56 ) |
                   ( ( src & 0x000000000000ff00 ) << 40 ) |
                   ( ( src & 0x0000000000ff0000 ) << 24 ) |
                   ( ( src & 0x00000000ff000000 ) <<  8 ) |
                   ( ( src & 0x000000ff00000000 ) >>  8 ) |
                   ( ( src & 0x0000ff0000000000 ) >> 24 ) |
                   ( ( src & 0x00ff000000000000 ) >> 40 ) |
                   ( ( src & 0xff00000000000000 ) >> 56 );
#endif
        }

        /** Swap the bytes of a little endian 16 bit value for big endian */
        static UInt16 SwapLE16( UInt16 src )
        {
#if ( AGGOO_ENDIAN == AGGOO_ENDIAN_LITTLE )
            return src;
#else
            return Swap16( src );
#endif
        }

        /** Swap the bytes of a big endian 16 bit value for little endian */
        static UInt16 SwapBE16( UInt16 src )
        {
#if ( AGGOO_ENDIAN == AGGOO_ENDIAN_BIG )
            return src;
#else
            return Swap16( src );
#endif
        }

        /** Swap the bytes of a little endian 32 bit value for big endian */
        static UInt32 SwapLE32( UInt32 src )
        {
#if ( AGGOO_ENDIAN == AGGOO_ENDIAN_LITTLE )
            return src;
#else
            return Swap32( src );
#endif
        }

        /** Swap the bytes of a big endian 32 bit value for little endian */
        static UInt32 SwapBE32( UInt32 src )
        {
#if ( AGGOO_ENDIAN == AGGOO_ENDIAN_BIG )
            return src;
#else
            return Swap32( src );
#endif
        }

        /** Swap the bytes of a little endian 64 bit value for big endian */
        static UInt64 SwapLE64( UInt64 src )
        {
#if ( AGGOO_ENDIAN == AGGOO_ENDIAN_LITTLE )
            return src;
#else
            return Swap64( src );
#endif
        }

        /** Swap the bytes of a big endian 64 bit value for little endian */
        static UInt64 SwapBE64( UInt64 src )
        {
#if ( AGGOO_ENDIAN == AGGOO_ENDIAN_BIG )
            return src;
#else
            return Swap64( src );
#endif
        }

    }; // class EndianSwap

} // namespace AggOO

#endif  // AggOOEndianSwap_H_
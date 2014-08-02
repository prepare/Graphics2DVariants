
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
// $Id: AggOOStringFormat.h,v 1.1 2006/11/01 22:41:01 dratek Exp $
//
// AggOOStringFormat.h -- Contains information regarding the formatting of strings
//
// Author       : Chad M. Draper
// Date         : October 25, 2006
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2006-10-25      Initial implementation
//

#ifndef AggOOStringFormat_H_
#define AggOOStringFormat_H_

#include "AggOOBase.h"
#include "AggOOEnums.h"
#include "AggOOTypes.h"

namespace AggOO
{
    /** @class StringFormat
    */
    class _AggOOExport StringFormat
    {
    protected:
        //! Set the status
        Status SetStatus( Status status ) const
        {
            if ( status != Ok )
                return ( mLastStatus = status );
            return status;
        }
        mutable Status mLastStatus;

        Status copyFrom( const StringFormat* src );

    private:
        StringAlignment     mStringAlignment;

    public:
        friend class Graphics;
        friend class GraphicsPath;

        StringFormat( const StringFormat* format );
        StringFormat( INT formatFlags, LANGID language = LANG_NEUTRAL );

        ~StringFormat(void);

        //! Get the last status flag
        Status GetLastStatus() const        { return mLastStatus; }

        //! Create a clone of this StringFormat
        StringFormat* Clone() const;

        //! Get the alignment for the string
        StringAlignment GetAlignment() const
        {
            return mStringAlignment;
        }
        //! Set the alignment for the string
        Status SetAlignment( StringAlignment align )
        {
            mStringAlignment = align;
            return SetStatus( Ok );
        }


    /**
        // TODO: The following exists in GDI+, but not AggOO, yet.

        //! Create a generic, default StringFormat
        static const StringFormat* GenericDefault();
        //! Create a generic, typographic StringFormat
        static const StringFormat* GenericTypographic();

        //! Get the language for the digits substituted by Western European digits
        LANGID GetDigitSubstitutionLanguage(void) const;
        //! Set the language for the digits substituted by Western European digits
        Status SetDigitSubstitution( LANGID language, StringDigitSubstitute substitute );

        //! Get how digits are to be substituted
        StringDigitSubstitute GetDigitSubstitutionMethod(void) const;
        //! Get how digits are to be substituted
        Status GetDigitSubstitutionMethod( LANGID language, StringDigitSubstitute substitute );

        //! Get the flags indicating formatting
        INT GetFormatFlags(void) const;
        //! Set the formatting flags 
        Status SetFormatFlags( INT flags );

        //! Get an indication as to what formatting is to be done when a hotkey
        //! character ('&') is encountered
        HotkeyPrefix GetHotkeyPrefix(void) const;
        Status GetHotkeyPrefix( HotkeyPrefix hotkeyPrefix );

        //! Get the line alignment in relation to the layout rectangle
        StringAlignment GetLineAlignment(void) const;
        //! Set the line alignment in relation to the layout rectangle
        Status GetLineAlignment( StringAlignment align );

        //! Get the number of measurable character ranges currently set
        INT GetMeasurableCharacterRangeCount(void) const;
        //! Set the number of measurable character ranges currently set
        Status SetMeasurableCharacterRange( INT count, const CharacterRange* ranges );

        //! Get the number of tab-stop offsets
        INT GetTabStopCount(void) const;

        //! Get the offsets of the tab-stops
        Status GetTabStops( INT count, REAL* firstTabOffset, REAL* tabStops ) const;
        //! Set the offsets of the tab-stops
        Status SetTabStops( REAL firstTabOffset, INT count, const REAL* tabStops );

        //! Get the style of trimming used when a string is too large for the layout
        //! rectangle
        StringTrimming GetTrimming(void) const;
        Status SetTrimming( StringTrimming trimming );

    */
    };

} // namespace

#endif  // AggOOStringFormat_H_

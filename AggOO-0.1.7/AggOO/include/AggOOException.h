
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


/*! $Id: AggOOException.h,v 1.1 2008/11/07 13:35:38 dratek Exp $
 *  @file   AggOOException.h
 *  @author Chad M. Draper
 *  @date   November 6, 2008
 *  @brief  Overrides the std::exception to provide information about an error.
 *  
 */

#ifndef AggOOException_H_
#define AggOOException_H_

#include <exception>
#include <sstream>

#include "AggOOPlatform.h"
#include "AggOOTypes.h"

namespace AggOO
{
    /** @class Exception
        The Exception class is derived from std::exception, and overrides the
        'what' method so a textual representation of the exception may be
        obtained.
    */
    class _AggOOExport Exception : std::exception
    {
    private:
        STRING     mText;

    public:
        /** Constructor */
        Exception( const STRING& text ) : mText( text )    { }
        Exception( const Exception& src ) : mText( src.mText )  { }

        /** Assignment operator */
        Exception& operator=( const Exception& src )
        {
            mText = src.mText;
            return *this;
        }

        /** Override the 'what' method to return the string */
        const char* what() const
        {
            return mText.c_str();
        }
    };

} // namespace AggOO

#endif  // AggOOException_H_

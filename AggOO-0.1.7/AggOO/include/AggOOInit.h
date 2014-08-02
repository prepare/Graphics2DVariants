
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
// $Id: AggOOInit.h,v 1.2 2006/11/03 21:07:28 dratek Exp $
//
// AggOOInit.h -- Initialization for AggOO
//
// Author       : Chad M. Draper
// Date         : October 18, 2006
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2006-10-20      Initial implementation
//

#ifndef AggOOInit_H_
#define AggOOInit_H_

#include "AggOOTypes.h"

namespace AggOO
{
    /** @remarks
        GDI+ requires initialization routines, but AGG does not.  Since the idea
        of AggOO is to be able to replace GDI+ by simply changing the namespace,
        these "initialization" methods and structures are provided.  They
        actually do nothing (since AggOO does not require any extra
        initializing) and are merely present for the cases of converting GDI+ ->
        AggOO.
    */
    /** Space-holder */
    struct GdiplusStartupInput
    {
    };

    /** Space-holder */
    struct GdiplusStartupOutput
    {
    };

    /** Space-holder */
    inline Status GdiplusStartup(
        ULONG_PTR *token,
        const GdiplusStartupInput *input,
        GdiplusStartupOutput *output)
    {
        *token = 0;
        return Ok;
    }

    /** Space-holder */
    inline VOID GdiplusShutdown(ULONG_PTR token)
    {
    }


} // namespace AggOO

#endif  // AggOOInit_H_

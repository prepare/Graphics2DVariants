
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
// $Id: AggOO.h,v 1.11 2009/02/26 22:13:37 dratek Exp $
//
// AggOO.h -- Main header file for the AggOO library.
//
// Author       : Chad M. Draper
// Date         : August 31, 2006
// Description  : Includes the entire library.
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2006-08-31      Initial implementation
//

#ifndef AggOO_H_
#define AggOO_H_

/**
    @mainpage   AggOO - The GDI+ like front-end for Anti-Grain Geometry

    @section    Introduction
        AggOO is an Object-Oriented library written in platform independent C++.
        Its purpose is to provide a higher-level front-end for <a href="http://www.antigrain.com">Anti-Grain Geometry</a>
        which resembles Microsoft GDI+.

    @section    Installation
        Before building AggOO, Anti-Grain Geometry must be built as a release
        and debug static library.  For more details, refer to the <a href="http://dratek.com/component/option,com_openwiki/Itemid,52/id,aggoo:building_agg/">wiki</a>
        at http://www.dratek.com.

        Download the latest version of AggOO from
        http://www.sourceforge.net/projects/aggoo.  To build and install the
        library, make sure the 'include' and 'lib' directories have been added
        to your compiler and linker paths.  Open the appropriate solution or
        workspace file for your environment, and build everything.  This should
        generate release and debug versions of the static library.

    @section    Usage
        Using AggOO is very similar to using GDI+.  There is a 'tests' directory
        with some examples you may refer to for help getting started (hopefully,
        there will be actual tutorials someday...)

    @section    Notes
        <UL>
            <LI>When using on Windows platforms, <code>windows.h</code> should
            be included before including any AggOO files.  If being used along
            with GDI+, be sure to include AggOO prior to including the GDI+
            header.
        </UL>
*/
/*
    When using this on windows, windows.h should be included before including
    the AggOO files.  If using GDI+, be sure to include AggOO before including
    GDI+.  This is to accommodate the nasty workarounds where the developers of
    the Windows SDK didn't know what they were doing ;-).
*/
#ifdef _GDIPLUS_H
#error GDI+ has been detected.  Make sure to include AggOO before including GDI+.
#endif


/* @namespace AggOO

    @remarks
        I'm not sure how far this will develop.  It would be nice to fully 
        emulate the GDI+ library using <a href="http:://www.antigrain.com"> 
        Anti-Grain Geometry</a>, which is what AggPlus is attempting to do. 
        Unfortunately, at this time, AggPlus is not very thorough, and it hasn't 
        been updated in months.  My priorities at this time are for the ability 
        to render using plain colors, where the alpha is not premultiplied. 
        Most work in AGG uses premultiplied alpha, which is fine when the entire 
        project is rendered with AGG, but when trying to render a transpareny 
        layer for an application such as PhotoShop, premultiplied alpha is a 
        bad thing, and causes halos around the anti-aliased drawings.  For now, 
        I just want to put together a simple API so I can render some shapes 
        using different brushes.  Over time, it might evolve into an ugly 
        monster that does a more thorough job of imitating GDI+.
    @par
        Looking into the future, where this might need to be a better GDI+ 
        replacement, it would be nice if the methods can be interchanged by 
        simply changing the namespace.  I will attempt to use similar naming 
        conventions as GDI+ whenever possible (or I can remember to do so.)  The 
        namespace will be AggOO (I don't want to create confusion with 
        AggPlus, which is an excellent project, just not to my current needs.)
*/

#include "AggOOPlatform.h"

#include "AggOOPrereqs.h"

// Include all necessary headers.  Typically, one will just need to include
// AggOO.h in order to include the entire library.

#include "AggOOAbstractPixelFormat.h"
#include "AggOOAbstractRendererBase.h"
#include "AggOOAbstractScanlineRenderer.h"
#include "AggOOBase.h"
#include "AggOOBitmap.h"
#include "AggOOBitmapExt.h"
#include "AggOOBrush.h"
#include "AggOOColor.h"
#include "AggOOColorExt.h"
#include "AggOOColorMatrix.h"
#include "AggOOEndianSwap.h"
#include "AggOOEnums.h"
#include "AggOOFileParseRenderer.h"
#include "AggOOGraphics.h"
#include "AggOOGraphicsExt.h"
#include "AggOOGraphicsPath.h"
#include "AggOOGraphicsPathCompound.h"
#include "AggOOGraphicsPathExt.h"
#include "AggOOImage.h"
#include "AggOOImageAttributes.h"
#include "AggOOInit.h"
#include "AggOOMath.h"
#include "AggOOMatrix.h"
#include "AggOOPen.h"
#include "AggOOPixelFormats.h"
#include "AggOOStringUtil.h"
#include "AggOOSharedPtr.h"
#include "AggOOTypes.h"
//#include "AggOOVectorShapeData.h"
//#include "AggOOVectorShapePath.h"
//#include "AggOOVectorShapeParser.h"

#endif  // AggOO_H_

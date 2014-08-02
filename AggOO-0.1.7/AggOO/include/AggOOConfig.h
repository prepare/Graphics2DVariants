
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
// $Id: AggOOConfig.h,v 1.4 2008/10/24 14:36:31 dratek Exp $
//
// AggOOConfig.h -- Configuration file for the AggOO library.
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

#ifndef AggOOConfig_H_
#define AggOOConfig_H_

///
/// Specify that floating point values should use floats or doubles.  In order
/// to use double precision, set AGGOO_PRECISION_DOUBLE to 1.  By default, float
/// is used.<br>
///
#ifndef AGGOO_PRECISION_DOUBLE
#define AGGOO_PRECISION_DOUBLE 0
#endif

///
/// Specify if the library is built with dynamic linkage.  In order to use
/// dynamic libraries, define AGGOO_DYNAMIC_LINKAGE to be 1.
///
#ifndef AGGOO_DYNAMIC_LINKAGE
#define AGGOO_DYNAMIC_LINKAGE 0
#endif

///
/// Specify that AggOO should use the DevIL library for loading images.  If this
/// is not defined, then AggOO will NOT load image files.
///
/// DevIL can be downloaded from http://openil.sourceforge.net/
///
#ifndef AGGOO_ENABLE_IMAGE_LOADING
#define AGGOO_ENABLE_IMAGE_LOADING 1
#endif

///
/// Specify that AggOO should use the FileParse library for loading vector
/// graphics files.  If this is not defined, then AggOO will NOT load vector
/// files (swf, svg, ai.)
///
/// FileParse may be downloaded from http://www.sourceforge.net/projects/vectorfileparse.
///
#ifndef AGGOO_ENABLE_VECTOR_LOADING
#define AGGOO_ENABLE_VECTOR_LOADING 1
#endif

#endif  // AggOOConfig_H_

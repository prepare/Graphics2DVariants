
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


$Id: AggOO_Readme.txt,v 1.6 2008/10/24 14:36:31 dratek Exp $

AggOO_Readme.txt -- Readme file for the AggOO library


Author: Chad M. Draper


Description:
------------
The Anti-Grain Geometry (http://www.antigrain.com) library is quite powerful and
useful for rendering 2D images.  However, using it is a big pain since it is
a low-level library.  There is a wrapper API called AggPlus
(http://www.point.com.mk/aggplus) which attempts to emulate GDI+ using AGG as
the graphics backend.  This shows great promise, and actually works for simple
uses quite well.  However, progress seems slow (current version is about 5
months old) and it doesn't do what I need it to do right now, which is render
transparent layers using plain colors (not premultiplied alpha) so I can use it
in Combustion layers.  I first thought about modifying the AggPlus code, but
the problem is it would take a while to figure it out, and then there is the
issue of maintenance.  What it comes down to the most is that I think I can
catch up to AggPlus quick enough.  Yeah right.

My primary development target is Windows, using Visual Studio .Net 2002.  I also
have access to Visual Studio 2005 and Code::Blocks using MinGW as the compiler,
so I will be able to test under those environments.  I don't know how much I
personally will be able to work on Macintosh or Linux versions, but the main
code base should be fairly portable.  The demo library will need some
modifications that hopefully someone can figure out.

I should point out that my primary focus is to implement the features that I
need.  There are a lot of features in GDI+ that I rarely, if ever, use, so those
will be very low priority for me to implement.  For example, I've not used the
ability to load an image from a stream, so that is something I'm not looking to
implement myself.  I have occasionally used the loading and saving of image
files, so that might be something I implement in the future.  I really need to
put together a road map of features I intend AggOO to have...


REQUIREMENTS
------------
AggOO requires AGG, so make sure AGG is built and working on the platform. Refer
to the Anti-Grain (http://www.antigrain.com) specs for more information.
Although there are demos that use GDI+ on Windows, the library itself does not
require GDI+, so it should build on non-Windows platforms as well.

By default, AggOO comes configured to load image files and vector graphics files.
These require the external libraries DevIL (for raster graphics) and
VectorFileParse (for vector graphics files, such as swf, svg, and ai.)  DevIL
can be obtained from http://openil.sourceforge.net, and VectorFileParse (a sister
project of AggOO, also written by me) is available from
http://www.sourceforge.net/projects/vectorfileparse.  These can be excluded from
AggOO (if you do not need to the ability to open external files) by changing the
respective enable flags to 0 in the AggOOConfig.h file before compiling.

COMPILE
-------
Compiling AggOO should be fairly straight forward, even if it isn't.  AggOO
expects AGG to be built as a static library, and instructions for doing this
can be found at http://www.dratek.com/wiki/9.html?Itemid=80.
It's actually pretty simple: create a static library project and add all of the
files to it.  Note that in order to use svg files, the files need to be copied
from examples/svg_viewer/ to include/svg and src/svg.  In reality, this wouldn't
HAVE to be done, but I think it makes the directory structure nicer than using
the example path.  Although I mention building AggOO as a dynamic library below, it
is not currently implemented (well, it hasn't been tested too much anyway.)

Here's a quick run-through on the process of getting everything built and
ready to use.

1.) Make sure the requirements are met.

2.) Setup the environment variables.  In Visual Studio, this should be under
    "Tools->Options...->Projects->VC++ Directories".  Add the path to the
    include files ("AggOO/include") and the library files ("AggOO/lib.")  In
    Code::Blocks, go to "Settings->Compiler", click the "Directories" tab and
    add the appropriate paths.  For other IDEs, consult your documentation.

3.) Load the appropriate project file.  For Win32 platforms, these can be found
    in the "AggOO/scripts/Win32" directory.  Other platforms will have similar
    directories.

4.) Build everything ("Build->Batch Build..." on VC++.)  This will build all
    projects, and place the library files in "AggOO/lib" and the DLL files in
    "AggOO/bin" as well as in "AggOO/tests/bin" for the test projects.  The test
    programs will be placed in "AggOO/tests/bin."

NOTE: The generated library names will be AggOO.(a|lib) or AggOO_d.(a|lib) for
      static builds, and AggOO_DLL.(a|lib) or AggOO_DLL_d.(a|lib) for dynamic
      builds.

NOTE: When building with dynamic linking, be sure to define
	  AGGOO_DYNAMIC_LINKAGE in the preprocess settings.

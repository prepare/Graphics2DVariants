
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

/*! $Id: AggOOKnot.h,v 1.2 2008/08/13 22:08:29 dratek Exp $
 *  @file   AggOOKnot.h
 *  @author Chad M. Draper
 *  @date   March 26, 2008
 *  @brief  Defines a node that is used as a control point in a spline.
 *  
 */

#ifndef AggOOKnot_H_
#define AggOOKnot_H_

#include "AggOOPlatform.h"
#include "AggOOEnums.h"
#include "AggOOTypes.h"
//#include "AggOOSharedPtr.h"

//#include <vector>

namespace AggOO
{
    class Spline;

    /** @class Knot
        A node that is used as a control point for a spline.
    */
    class _AggOOExport Knot
    {
        friend class Spline;

    public:
        enum KnotType { Corner, Curve, Symmetric }; /**< Indicates how adjusting the node's handles will effect the "other" handle. */

    private:
        KnotType    mKnotType;      /**< This node's type */
        PointF      mPosition;      /**< Position of this node */
        PointF      mInVector;      /**< Vector coming into the node */
        PointF      mOutVector;     /**< Vector going out of the node */

    public:
        /** Default constructor */
        Knot(void);
        /** Constructor */
        Knot( KnotType type );
        /** Constructor */
        Knot( const PointF& pos );
        /** Constructor */
        Knot( const PointF& pos, const PointF& inVec, const PointF& outVec );
        /** Constructor */
        Knot( KnotType type, const PointF& pos, const PointF& inVec, const PointF& outVec );
        /** Destructor */
        ~Knot(void);

        /** Get the node type */
        KnotType GetKnotType() const            { return mKnotType; }
        /** Set the node type */
        void SetKnotType( KnotType type );

        /** Get the node's position */
        const PointF& GetPosition() const       { return mPosition; }
        /** Set the node's position */
        void SetPosition( const PointF& pos );

        /** Get the node's in vector */
        const PointF& GetInVector() const       { return mInVector; }
        /** Set the node's in vector */
        void SetInVector( const PointF& vec );

        /** Get the node's out vector */
        const PointF& GetOutVector() const      { return mOutVector; }
        /** Set the node's out vector */
        void SetOutVector( const PointF& vec );

        /** Determine if this knot is equal to another */
        bool Equals( const Knot& other ) const;

        /** Determine if this knot equals another, based on position */
        bool PositionEquals( const Knot& other ) const;

        /** Write to a stream */
        inline friend std::ostream& operator<<( std::ostream& stream, const Knot& src )
        {
            std::ostream::sentry opfx(stream);
            if ( !opfx )
                return stream;

            stream << "Knot{ Type: " << src.mKnotType << ", Position: " << src.mPosition << ", InVector: " << src.mInVector << ", OutVector: " << src.mOutVector << " }\n";

            return stream;
        }

    }; // Knot

} // namespace AggOO

#endif // AggOOKnot_H_

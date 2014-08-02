
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


/*! $Id: AggOOSharedPtr.h,v 1.1 2008/02/07 17:41:37 dratek Exp $
 *  @file   AggOOSharedPtr.h
 *  @author Chad M. Draper
 *  @date   January 25, 2008
 *  @brief  Implements a reference-counted "smart" pointer so that dependencies
 *          on other libraries (such as Boost) can be avoided.
 *  
 */

#ifndef AggOOSharedPtr_H_
#define AggOOSharedPtr_H_

#include <assert.h>

namespace AggOO
{
    /**
        The SharedPtr is a reference-counted pointer, used where implicit
        destruction is required.
        @remarks
        This is a basic shared pointer that uses a reference count to know
        when to delete a pointer.
    */
    template < class T >
    class SharedPtr
    {
    protected:
        T*  pData;                          // Pointer data
        unsigned int* pUseCount;            // Number of references to this pointer

    public:
        /**
            Constructor, creates a null pointer.
            @remarks
            <b>Important!</b> bind() must be called before using SharedPtr.
        */
        SharedPtr() : pData( 0 ), pUseCount( 0 )
        {
        }

        explicit SharedPtr( T* data ) : pData( data ), pUseCount( new unsigned int( 1 ) )
        {
        }

        SharedPtr( const SharedPtr& ptr )
            : pData( 0 ), pUseCount( 0 )
        {
            pData = ptr.pData;
            pUseCount = ptr.pUseCount;
            if ( pUseCount )
                ++(*pUseCount);
        }

        SharedPtr& operator=( const SharedPtr& ptr )
        {
            if ( pData == ptr.pData )
                return *this;
            release();

            pData = ptr.pData;
            pUseCount = ptr.pUseCount;
            if ( pUseCount )
                ++(*pUseCount);

            return *this;
        }

        ~SharedPtr()
        {
            release();
        }

        inline T& operator*() const
        {
            assert( pData );
            return *pData;
        }
        inline T* operator->() const
        {
            assert( pData );
            return pData;
        }
        inline T* get() const
        {
            return pData;
        }

        /**
            Bind data to the pointer.
            @remarks
            Assumes the pointer is not initialized.
        */
        void bind( T* data )
        {
            assert( !pData && !pUseCount );
            pUseCount = new unsigned int( 1 );
            pData = data;
        }

        inline bool isUnique() const
        {
            assert( pUseCount );
            return *pUseCount == 1;
        }
        inline unsigned int useCount() const
        {
            assert( pUseCount );
            return *pUseCount;
        }
        inline unsigned int* useCountPtr() const
        {
            return pUseCount;
        }

        inline T* getPointer() const    { return pData; }

        inline bool isNull() const      { return pData == 0; }

        inline void setNull()
        {
            if ( pData )
            {
                release();
                pData = 0;
                pUseCount = 0;
            }
        }

    protected:
        /** Release the pointer
        */
        void release()
        {
            bool destroyThis = false;

            if ( pUseCount )
            {
                if ( --(*pUseCount) == 0 )
                {
                    destroyThis = true;
                }
            }
            if ( destroyThis )
            {
                destroy();
            }
        }

        /**
            Destroy the pointer
            @remarks
            <i>If this causes a crash, there were pointers that were not freed
            prior to shutting down.</i>
        */
        virtual void destroy()
        {
            delete pData;       pData = 0;
            delete pUseCount;   pUseCount = 0;
        }

    }; // class SharedPtr

    template< class T, class U > inline bool operator==( SharedPtr<T> const& a, SharedPtr<U> const& b )
    {
        return ( a.get() == b.get() );
    }

    template< class T, class U > inline bool operator!=( SharedPtr<T> const& a, SharedPtr<U> const& b )
    {
        return ( a.get() != b.get() );
    }

} // namespace AggOO

#endif  // AggOOSharedPtr_H_

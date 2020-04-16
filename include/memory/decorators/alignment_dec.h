/* 
 * This file is part of the MemoryDecorator Project (https://github.com/JPietrzykTUD/MemoryDecorator).
 * Copyright (c) 2020 Johannes Pietrzyk.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_DECORATORS_ALIGNMENT_DEC_H
#define TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_DECORATORS_ALIGNMENT_DEC_H

#include <utils/singleton.h>
#include <utils/literals.h>
#include <utils/string.h>
#include <utils/types.h>
#include <cstddef>
#include <cstring>
#include <string>



#include <iostream>

namespace tuddbs {
   template< class NestedDecorator >
   class alignment_decorator : public singleton< alignment_decorator< NestedDecorator > > {
         friend class singleton< alignment_decorator< NestedDecorator > >;

         static std::size_t get_own_size_needed( std::size_t alignment ) {
            return 2*sizeof( std::size_t ) + sizeof( std::size_t ) + alignment;
         }
      public:
         template< class... Args >
         static std::size_t get_size_needed( std::size_t alignment, Args... args ) {
            return get_own_size_needed( alignment ) + NestedDecorator::get_size_needed( args... );
         }

         template< class... Args >
         static void * decorate( void * const p, std::size_t size, std::size_t alignment, Args... args ) {
            byte * ptr = ( byte * ) NestedDecorator::decorate( p, args... );
            std::size_t ptr_value = ( std::size_t ) ( ptr + 3 * sizeof( std::size_t ) );
            std::size_t align_value = sizeof( std::size_t ) + ( alignment - ( ptr_value & ( alignment - 1 ) ) );
            std::memcpy( ( void * ) ptr, ( void const * ) &size, sizeof( std::size_t ) );
            std::memcpy( ( void * ) ( ptr + sizeof( std::size_t ) ), ( void const * ) &alignment, sizeof( std::size_t ) );
            ptr += 2 * sizeof( std::size_t ) + align_value;
            ptr -= sizeof( std::size_t );
            std::memcpy( ( void * ) ( ptr ), ( void const * ) &align_value, sizeof( std::size_t ) );
            ptr += sizeof( std::size_t );
            return ( void * ) ptr;
         }

         template< class... Args >
         static void * verbose( void * const p, Args... args ) {
            byte * ptr = ( byte * ) NestedDecorator::verbose( p, args... );
            byte * const ptr_orig = ptr;
            byte tmp[ sizeof( std::size_t ) ];
            std::memcpy( ( void * ) tmp, ( void const * )ptr, sizeof( std::size_t ) );
            std::cerr << "{ Allocated Size: " <<  *(( std::size_t *) tmp) << ". ";
            std::memcpy( ( void * ) tmp, ( void const * )( ptr + sizeof( std::size_t ) ), sizeof( std::size_t ) );
            std::size_t alignment = *(( std::size_t *) tmp);
            std::cerr << "Align to: " <<  alignment << ". ";
            std::size_t ptr_value = ( std::size_t ) ( ptr_orig + 3 * sizeof( std::size_t ) );
            std::size_t align_value = sizeof( std::size_t ) + ( alignment - ( ptr_value & ( alignment - 1 ) ) );
            ptr += 2 * sizeof( std::size_t ) + align_value;
            ptr -= sizeof( std::size_t );
            std::memcpy( ( void * ) tmp, ( void const * )( ptr ), sizeof( std::size_t ) );
            std::cerr << "Aligned by: " <<  *(( std::size_t *) tmp) << ". }";
            ptr += sizeof( std::size_t );
            return ( void * ) ptr;
         }

         static void * get_root( void * const p ) {
            byte * ptr = ( byte * ) p;
            byte tmp[ sizeof( std::size_t ) ];
            std::memcpy( ( void * ) tmp, ( void const * )( ptr - sizeof( std::size_t ) ), sizeof( std::size_t ) );
            ptr -= ( 2*sizeof( std::size_t ) + ( *(( std::size_t *) tmp) ) );
            return NestedDecorator::get_root( ( void * ) ptr );
         }

         static std::size_t get_size_from_ptr( void * const p ) {
            byte * ptr = ( byte * ) p;
            byte tmp[ sizeof( std::size_t ) ];
            std::memcpy( ( void * ) tmp, ( void const * )( ptr - sizeof( std::size_t ) ), sizeof( std::size_t ) );
            ptr -= ( 2*sizeof( std::size_t ) + ( *(( std::size_t *) tmp) ) );
            std::memcpy( ( void * ) tmp, ( void const * )ptr, sizeof( std::size_t ) );
            return *(( std::size_t *) tmp);
         }
   };
}

#endif //TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_DECORATORS_ALIGNMENT_DEC_H

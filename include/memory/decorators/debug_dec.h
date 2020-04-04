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
#ifndef TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_DECORATORS_DEBUG_DEC_H
#define TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_DECORATORS_DEBUG_DEC_H

#include <utils/singleton.h>
#include <utils/literals.h>
#include <memory/types.h>
#include <cstddef>
#include <cstring>
#include <string>

#include <iostream>

namespace tuddbs {
   template< class NestedDecorator >
   class debug_decorator : public singleton< debug_decorator< NestedDecorator > > {
         friend class singleton< debug_decorator< NestedDecorator > >;

      private:
         static constexpr std::size_t offset_from_start = 2_KB;
         static constexpr std::size_t debug_buffer_size = 4_KB;

         static std::size_t get_own_size_needed( void ) {
            return debug_buffer_size;
         }

      public:
         template< class... Args >
         static std::size_t get_size_needed( Args... args ) {
            return get_own_size_needed( ) + NestedDecorator::get_size_needed( args... );
         }

         template< class... Args >
         static void * decorate( void * const p, char const * file_name, std::size_t line_number, Args... args ) {
            std::byte * ptr = ( std::byte * ) NestedDecorator::decorate( p, args... );
            std::byte * ptr_orig = ptr;
            ptr += offset_from_start;
            std::string debug_string( file_name );
            debug_string += ":" + std::to_string( line_number );
            std::size_t length = debug_string.length();
            std::memcpy( ( void * ) ptr, ( void const * ) &length, sizeof( std::size_t ) );
            std::memcpy( ( void * ) ( ptr + sizeof( std::size_t ) ), ( void const * )debug_string.c_str(), debug_string.length() );
            ptr_orig += ( get_own_size_needed( ) );
            return ( void * ) ptr_orig;
         }

         template< class... Args >
         static void * verbose( void * const p, Args... args ) {
            std::byte * ptr = ( std::byte * ) NestedDecorator::verbose( p, args... );
            std::byte * ptr_orig = ptr;
            ptr += offset_from_start;
            std::byte tmp[ sizeof( std::size_t ) ];
            std::memcpy( ( void * ) tmp, ( void const * )ptr, sizeof( std::size_t ) );
            std::size_t length = *(( std::size_t *) tmp);
            std::string debug_string( ( char * const ) ( ptr + sizeof( std::size_t ) ), ( char * const ) ( ptr + sizeof( std::size_t ) + length ) );
            std::cerr << "Allocated here: " << debug_string << ". ";
            ptr_orig += ( get_own_size_needed( ) );
            return ( void * ) ptr_orig;
         }

         static void * get_root( void * const p ) {
            std::byte * ptr = ( std::byte * ) p;
            ptr -= ( get_own_size_needed( ) );
            return NestedDecorator::get_root( ( void * const ) ptr );
         }
   };
}

#endif //TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_DECORATORS_DEBUG_DEC_H

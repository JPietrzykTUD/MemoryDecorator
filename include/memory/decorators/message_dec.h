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
#ifndef TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_DECORATORS_MESSAGE_DEC_H
#define TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_DECORATORS_MESSAGE_DEC_H

#include <utils/md_singleton.h>
#include <utils/md_literals.h>
#include <utils/md_string.h>
#include <utils/md_types.h>
#include <utils/md_preprocessor.h>
#include <cstddef>
#include <cstring>
#include <string>



#include <iostream>

namespace tuddbs {
   template< class NestedDecorator >
   class message_decorator : public singleton< message_decorator< NestedDecorator > > {
         friend class singleton< message_decorator< NestedDecorator > >;

      private:
         static constexpr std::size_t offset_from_start = 2_KB;
         static constexpr std::size_t message_buffer_size = 8_KB;

         static std::size_t get_own_size_needed( void ) {
            return message_buffer_size;
         }

      public:
         template< class... Args >
         static std::size_t get_size_needed( UNUSED Args... args ) {
            return get_own_size_needed( ) + NestedDecorator::get_size_needed( );
         }

         template< class... Args >
         static void * decorate( void * const p, Args... args ) {
            byte * ptr = ( byte * ) NestedDecorator::decorate( p );
            byte * ptr_orig = ptr;
            ptr += offset_from_start;
            std::string message_str = concat( { toString( args )... } );
            std::size_t length = message_str.length();
            std::memcpy( ( void * ) ptr, ( void const * ) &length, sizeof( std::size_t ) );
            std::memcpy( ( void * ) ( ptr + sizeof( std::size_t ) ), ( void const * )message_str.c_str(), message_str.length() );
            ptr_orig += ( get_own_size_needed( ) );
            return ( void * ) ptr_orig;
         }


         template< class... Args >
         static void * verbose( void * const p, UNUSED Args... args ) {
            byte * ptr = ( byte * ) NestedDecorator::verbose( p );
            byte * ptr_orig = ptr;
            ptr += offset_from_start;
            byte tmp[ sizeof( std::size_t ) ];
            std::memcpy( ( void * ) tmp, ( void const * )ptr, sizeof( std::size_t ) );
            std::size_t length = *(( std::size_t *) tmp);
            std::string message_string( ( char * ) ( ptr + sizeof( std::size_t ) ), ( char * ) ( ptr + sizeof( std::size_t ) + length ) );
            std::cerr << "Additional Info: " << message_string << ". ";
            ptr_orig += ( get_own_size_needed( )  );
            return ( void * ) ptr_orig;
         }

         static void * get_root( void * const p ) {
            byte * ptr = ( byte * ) p;
            ptr -= ( get_own_size_needed( ) );
            return NestedDecorator::get_root( ( void * ) ptr );
         }
   };
}

#endif //TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_DECORATORS_MESSAGE_DEC_H

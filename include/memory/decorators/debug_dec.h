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

#include <utils/md_singleton.h>
#include <utils/md_literals.h>
#include <utils/md_types.h>
#include <cstddef>
#include <cstring>
#include <string>
#include <sstream>

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
            byte * ptr = ( byte * ) NestedDecorator::decorate( p, args... );
            byte * ptr_orig = ptr;
            ptr += offset_from_start;
            std::stringstream debug_stringstream;
//            std::cerr << file_name << " - " << line_number << "\n";
//            std::string debug_string( file_name );
//            std::cerr << file_name << "\n";
//            std::cerr << std::to_string(line_number) << "\n";
//            debug_string += ":" + std::to_string( line_number );
            debug_stringstream << file_name << ":" << line_number;
            std::string debug_string = debug_stringstream.str();
            std::size_t length = debug_string.length();
            std::memcpy( ( void * ) ptr, ( void const * ) &length, sizeof( std::size_t ) );
            std::memcpy( ( void * ) ( ptr + sizeof( std::size_t ) ), ( void const * )debug_string.c_str(), debug_string.length() );
            ptr_orig += ( get_own_size_needed( ) );
            return ( void * ) ptr_orig;
         }

         template< class... Args >
         static void * verbose( void * const p, Args... args ) {
            byte * ptr = ( byte * ) NestedDecorator::verbose( p, args... );
            byte * ptr_orig = ptr;
            ptr += offset_from_start;
            byte tmp[ sizeof( std::size_t ) ];
            std::memcpy( ( void * ) tmp, ( void const * )ptr, sizeof( std::size_t ) );
            std::size_t length = *(( std::size_t *) tmp);
            std::string debug_string( ( char * const ) ( ptr + sizeof( std::size_t ) ), ( char * const ) ( ptr + sizeof( std::size_t ) + length ) );
            std::cerr << "Allocated here: " << debug_string << ". ";
            ptr_orig += ( get_own_size_needed( ) );
            return ( void * ) ptr_orig;
         }

         static void * get_root( void * const p ) {
            byte * ptr = ( byte * ) p;
            ptr -= ( get_own_size_needed( ) );
            return NestedDecorator::get_root( ( void * const ) ptr );
         }
   };
}

#endif //TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_DECORATORS_DEBUG_DEC_H

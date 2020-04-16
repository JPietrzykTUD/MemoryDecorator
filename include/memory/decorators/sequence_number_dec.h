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
#ifndef TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_DECORATORS_SEQUENCE_NUMBER_DECORATOR_H
#define TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_DECORATORS_SEQUENCE_NUMBER_DECORATOR_H

#include <utils/md_singleton.h>
#include <utils/md_literals.h>
#include <utils/md_types.h>
#include <cstddef>
#include <cstring>
#include <string>
#include <iostream>

namespace tuddbs {
   template< class NestedDecorator >
   class sequence_number_decorator : public singleton< sequence_number_decorator< NestedDecorator > > {
         friend class singleton< sequence_number_decorator< NestedDecorator > >;
      private:
         static constexpr std::size_t sequence_number_size = sizeof( std::size_t );
         std::size_t sequence_number;

         static std::size_t get_own_size_needed( void ) {
            return sequence_number_size;
         }

         sequence_number_decorator( ) : sequence_number{ 0 } { }
      public:
         std::size_t get_and_inc( void ) {
            return sequence_number++;
         }
         template< class... Args >
         static std::size_t get_size_needed( Args... args ) {
            return get_own_size_needed( ) + NestedDecorator::get_size_needed( args... );
         }
         template< class... Args >
         static void * decorate( void * const p, Args... args ) {
            byte * ptr = ( byte * ) NestedDecorator::decorate( p, args... );
            std::size_t val = sequence_number_decorator< NestedDecorator >::instance()->get_and_inc( );
            std::memcpy( ( void * ) ptr, ( void const * ) &val, sizeof( std::size_t ) );
            ptr += get_own_size_needed( );
            return ( void * ) ptr;
         }

         template< class... Args >
         static void * verbose( void * const p, Args... args ) {
            byte * ptr = ( byte * ) NestedDecorator::verbose( p, args... );
            byte tmp[ sizeof( std::size_t ) ];
            std::memcpy( ( void * ) tmp, ( void const * )ptr, sizeof( std::size_t ) );
            std::cerr << "Allocation #" <<  *(( std::size_t *) tmp) << ". ";
            ptr += get_own_size_needed( );
            return ptr;
         }

         static void * get_root( void * const p ) {
            byte * ptr = ( byte * ) p;
            ptr -= ( get_own_size_needed( ) );
            return NestedDecorator::get_root( ( void * const ) ptr );
         }
   };
}

#endif //TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_DECORATORS_SEQUENCE_NUMBER_DECORATOR_H

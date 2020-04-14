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
#ifndef TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_VALIDATORS_FENCE_VAL_H
#define TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_VALIDATORS_FENCE_VAL_H

#include <utils/singleton.h>
#include <utils/literals.h>
#include <memory/types.h>
#include <cstddef>
#include <cstring>
#include <string>
#include <iostream>
#include <tuple>

namespace tuddbs {
   template< class NestedValidator >
   class fence_validator : public singleton< fence_validator< NestedValidator > > {
         friend class singleton< fence_validator< NestedValidator > >;
      private:
         static constexpr uint64_t FENCE_VALUE = 0xC004C0FEEBADC0DE;
         static constexpr std::size_t fence_buffer_size = 16_KB;

         static std::size_t get_own_size_needed( void ) {
            return fence_buffer_size;
         }

      public:
         static std::size_t get_size_needed( void ) {
            return get_own_size_needed( ) + NestedValidator::get_size_needed( );
         }
         static ptr_t & decorate( ptr_t & p ) {
            p = NestedValidator::decorate( p );
            uint64_t * tmp = ( uint64_t * ) p.data();
            for( std::size_t i = 0; i < fence_buffer_size / sizeof( uint64_t ); ++i ) {
               tmp[ i ] = FENCE_VALUE;
            }
            p.advance( get_own_size_needed() );
            return p;
         }

         static void * decorate( void * const p ) {
            std::byte * ptr = ( std::byte * ) NestedValidator::decorate( p );
            uint64_t * tmp = ( uint64_t * ) ptr;
            for( std::size_t i = 0; i < fence_buffer_size / sizeof( uint64_t ); ++i ) {
               tmp[ i ] = FENCE_VALUE;
            }
            ptr += get_own_size_needed( );
            return ptr;
         }

         static bool validate( ptr_t & p ) {
            bool result = NestedValidator::validate( p );
            uint64_t * tmp = ( uint64_t * ) p.data();
            for( std::size_t i = 0; i < fence_buffer_size / sizeof( uint64_t ); ++i ) {
               if( tmp[ i ] != FENCE_VALUE ) {
                  result = false;
                  break;
               }
            }
            p.advance( fence_buffer_size );
            return ( result );
         }

         static std::tuple< bool, void * > validate( void * const p )  {
            bool result;
            void * ptr;
            std::tie( result, ptr ) = NestedValidator::validate( p );
            uint64_t * tmp = ( uint64_t * ) ptr;
            for( std::size_t i = 0; i < fence_buffer_size / sizeof( uint64_t ); ++i ) {
               if( tmp[ i ] != FENCE_VALUE ) {
                  result = false;
                  break;
               }
            }
            return std::make_tuple( result, ( void * ) ( ( std::byte * ) ptr + fence_buffer_size ) );
         }

         static ptr_t verbose( ptr_t & p ) {
            p = NestedValidator::verbose( p );
            uint64_t * tmp = ( uint64_t * ) p.data();
            bool everything_ok = true;
            bool corrupted_block = false;
            std::size_t lower = 0;
            for( std::size_t i = 0; i < fence_buffer_size / sizeof( uint64_t ); ++i ) {
               if( tmp[ i ] == FENCE_VALUE ) {
                  if( corrupted_block ) {
                     std::cerr << lower * sizeof( uint64_t ) << " - " << i * sizeof( uint64_t ) << ". ";
                     corrupted_block = false;
                  }
               } else {
                  if( !corrupted_block ) {
                     std::cerr << "Corrupted Bytes: ";
                     lower = i;
                     everything_ok = false;
                     corrupted_block = true;
                  }
               }
            }
            if( !everything_ok ) {
               if( corrupted_block ) {
                  std::cerr << lower * sizeof( uint64_t ) << " - " << fence_buffer_size << " (END). ";
               }
            }
            p.advance( fence_buffer_size );
            return p;
         }

         static void * verbose( void * const p ) {
            std::byte * ptr = ( std::byte * ) NestedValidator::verbose( p );
            uint64_t * tmp = ( uint64_t * ) ptr;
            bool everything_ok = true;
            bool corrupted_block = false;
            std::size_t lower = 0;
            for( std::size_t i = 0; i < fence_buffer_size / sizeof( uint64_t ); ++i ) {
               if( tmp[ i ] == FENCE_VALUE ) {
                  if( corrupted_block ) {
                     std::cerr << lower * sizeof( uint64_t ) << " - " << i * sizeof( uint64_t ) << ". ";
                     corrupted_block = false;
                  }
               } else {
                  if( !corrupted_block ) {
                     std::cerr << "Corrupted Bytes: ";
                     lower = i;
                     everything_ok = false;
                     corrupted_block = true;
                  }
               }
            }
            if( !everything_ok ) {
               if( corrupted_block ) {
                  std::cerr << lower * sizeof( uint64_t ) << " - " << fence_buffer_size << " (END). ";
               }
            }
            ptr += fence_buffer_size;
            return ptr;
         }

         static void * get_root( void * const p ) {
            std::byte * ptr = ( std::byte * ) p;
            ptr -= ( get_own_size_needed() );
            return NestedValidator::get_root( ( void * const ) ptr );
         }
   };
}

#endif //TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_VALIDATORS_FENCE_VAL_H

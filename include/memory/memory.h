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
#ifndef TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_MEMORY_H
#define TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_MEMORY_H


#include <memory/decorators/debug_dec.h>
#include <memory/decorators/sequence_number_dec.h>
#include <memory/decorators/message_dec.h>
#include <memory/decorators/alignment_dec.h>
#include <memory/decorators/null_dec.h>
#include <memory/validators/fence_val.h>
#include <memory/validators/null_val.h>
#include <utils/md_types.h>

#include <memory/allocators/dram_alloc.h>
#ifdef HAS_MCD
#include <memory/allocators/mcdram_alloc.h>
#endif

#include <cstddef>

namespace tuddbs {
#ifdef HAS_MCD
   using allocator_t = mcdram_allocator;
#else
   using allocator_t = dram_allocator;
#endif

#ifdef MEMORY_DEBUG
   using decorator_chain_t =
      alignment_decorator<
         debug_decorator<
            sequence_number_decorator<
               message_decorator<
                  null_decorator
               >
            >
         >
      >;
   using validator_chain_t =
      fence_validator<
         null_validator
      >;

   template< class... Args >
   void * acquire_impl(
      std::size_t size,
#ifdef HAS_MCD
      bool use_mcd,
#endif
      Args... args
   ) {
      std::size_t size_to_allocate = size + 2 * validator_chain_t::get_size_needed( ) + decorator_chain_t::get_size_needed( args... );
#ifdef HAS_MCD
      void * result  = allocator_t::instance()->allocate( size_to_allocate, use_mcd );
#else
      void * result  = allocator_t::instance()->allocate( size_to_allocate );
#endif
      result = validator_chain_t::decorate( result );
      result = decorator_chain_t::decorate( result, size, args... );
      validator_chain_t::decorate( ( void * ) ( ( byte * ) result + size ) );
      return result;
   }

   template< class... Args >
   bool inspect_impl( void * const p, UNUSED char const * file_name, UNUSED std::size_t line_number ) {
//      std::cout << "Inspect called: " << file_name << ":" << line_number << "\n";
      std::size_t requested_size = decorator_chain_t::get_size_from_ptr( p );
      void * const ptr_back = ( void * ) ( ( byte * ) p + requested_size );
      void * const ptr_front = validator_chain_t::get_root( decorator_chain_t::get_root( p ) );
      bool result_back, result_front;
      std::tie( result_back, std::ignore ) = validator_chain_t::validate( ptr_back );
      void * ptr;
      std::tie( result_front, ptr ) = validator_chain_t::validate( ptr_front );
      if(! ( result_back && result_front ) ) {
         std::cerr << "Memory Corruption:\n";
         std::cerr << "   [ ";
         ptr = decorator_chain_t::verbose( ptr );
         std::cerr << " ].\n   ";
         if( !result_front ) {
            std::cerr << "Got overflowed: ";
            validator_chain_t::verbose( ptr_front );
         }
         if( !result_back ) {
            std::cerr << "Overflowed: ";
            validator_chain_t::verbose( ptr_back );
         }
         std::cerr << "\n";
      }
      return ( result_back && result_front );
   }

   template< class... Args >
   void release_impl(
      void * const p,
#ifdef HAS_MCD
      bool use_mcd,
#endif
      char const * file_name, std::size_t line_number
   ) {
//      std::cout << "Release called: " << file_name << ":" << line_number << "\n";
      inspect_impl( p, file_name, line_number );
      void * const ptr_front = validator_chain_t::get_root( decorator_chain_t::get_root( p ) );
#ifdef HAS_MCD
      allocator_t::instance()->deallocate( ptr_front, use_mcd );
#else
      allocator_t::instance()->deallocate( ptr_front );
#endif
   }

#ifdef HAS_MCD
#  define acquire(size, use_mcdram, alignment) acquire_impl(size, use_mcdram, alignment, __FILE__, __LINE__)
#  define release_ptr( ptr, use_mcdram ) release_impl(ptr, use_mcdram, __FILE__, __LINE__)
#else
#  define acquire(size, alignment) acquire_impl(size, alignment, __FILE__, __LINE__)
#  define release_ptr( ptr ) release_impl(ptr, __FILE__, __LINE__)
#endif

#define inspect( ptr ) inspect_impl(ptr, __FILE__, __LINE__)
#else
   using decorator_chain_t =
      alignment_decorator<
         null_decorator
      >;

   template< class... Args >
   void * acquire_impl(
      std::size_t size,
#ifdef HAS_MCD
      bool use_mcd,
#endif
      Args... args
   ) {
      std::size_t size_to_allocate = size + decorator_chain_t::get_size_needed( args... );
#ifdef HAS_MCD
      void * result  = allocator_t::instance()->allocate( size_to_allocate, use_mcd );
#else
      void * result  = allocator_t::instance()->allocate( size_to_allocate );
#endif
      return decorator_chain_t::decorate( result, size, args... );
   }
   void release_impl(
      void * const p
#ifdef HAS_MCD
      , bool use_mcd
#endif
   ) {
      void * const ptr_front = decorator_chain_t::get_root( p ) ;
#ifdef HAS_MCD
      allocator_t::instance()->deallocate( ptr_front, use_mcd );
#else
      allocator_t::instance()->deallocate( ptr_front );
#endif
   }

#ifdef HAS_MCD
#  define acquire(size, use_mcdram, alignment) acquire_impl(size, use_mcdram, alignment)
#  define release_ptr( ptr, use_mcdram ) release_impl(ptr, use_mcdram)
#else
#  define acquire(size, alignment) acquire_impl(size, alignment)
#  define release_ptr( ptr ) release_impl(ptr)
#endif

#define inspect( ptr )
#endif


}



#endif //TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_MEMORY_H

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
#ifndef TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_ALLOCATORS_MCDRAM_ALLOC_H
#define TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_ALLOCATORS_MCDRAM_ALLOC_H

#include <utils/md_singleton.h>

#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <hbwmalloc.h>

namespace tuddbs {
   class mcdram_allocator : public singleton< mcdram_allocator > {
         friend class singleton< mcdram_allocator >;

      public:
         void * allocate( std::size_t size, bool use_mcd ) {
            if( use_mcd ) {
               return hbw_malloc( size );
            }
            return malloc( size );
         }

         void deallocate( void * const p, bool use_mcd ) {
            if( use_mcd ) {
               hbw_free( p );
            } else {
               free( p );
            }
         }
   };
}

#endif //TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_ALLOCATORS_MCDRAM_ALLOC_H

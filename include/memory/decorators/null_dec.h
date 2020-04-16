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
#ifndef TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_DECORATORS_NULL_DEC_H
#define TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_DECORATORS_NULL_DEC_H

#include <utils/md_singleton.h>
#include <utils/md_literals.h>
#include <utils/md_string.h>
#include <utils/md_types.h>
#include <cstddef>
#include <cstring>
#include <string>



#include <iostream>

namespace tuddbs {

   class null_decorator : public singleton< null_decorator > {
         friend class singleton< null_decorator >;

      public:
         static std::size_t get_size_needed( void ) {
            return 0;
         }

         template< class... Args >
         static void * decorate( void * const p, UNUSED Args... args ) { return p; }


         template< class... Args >
         static void * verbose( void * const p, UNUSED Args... args ) { return p; }

         static void * get_root( void * const p ) { return p; }
   };
}
#endif //TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_DECORATORS_NULL_DEC_H

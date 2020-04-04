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
#ifndef TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_VALIDATORS_NULL_VAL_H
#define TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_VALIDATORS_NULL_VAL_H

#include <utils/singleton.h>
#include <utils/literals.h>
#include <memory/types.h>
#include <cstddef>
#include <cstring>
#include <string>
#include <iostream>

namespace tuddbs {
   class null_validator : public singleton< null_validator > {
         friend class singleton< null_validator >;
      public:
         static std::size_t get_size_needed( void ) {
            return 0;
         }
         static ptr_t & decorate( ptr_t & p ) {
            return p;
         }
         static void * decorate( void * const p ) { return p; }

         static bool validate( ptr_t & p ) {
            return true;
         }

         static std::tuple< bool, void * > validate( void * const p ) { return std::make_tuple( true, p ); }

         static ptr_t verbose( ptr_t & p ) {
            return p;
         }

         static void * verbose( void * const p ) { return p; }

         static void * get_root( void * const p ) { return p; }
   };
}


#endif //TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_VALIDATORS_NULL_VAL_H

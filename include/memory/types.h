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
#ifndef TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_TYPES_H
#define TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_TYPES_H

#include <cstddef>
#include <utils/types.h>

namespace tuddbs {

   class ptr_t {
      public:
         inline void advance( std::size_t i ) {
            m_data += i;
         }
         inline void recede( std::size_t i ) {
            m_data -= i;
         }
         inline void reset( void ) {
            m_data = m_orig;
         }
      private:
         byte * m_orig;
         byte * m_data;
         std::size_t m_size;
      public:
         ptr_t( void * const p_p, std::size_t p_size ) : m_orig{ ( byte * ) p_p }, m_data{ m_orig },  m_size{ p_size } {}
         void * data() const { return ( void * ) m_data; }
         void * orig() const { return ( void * ) m_orig; }
         std::size_t size() const { return m_size; }
   };
   std::ostream & operator<<( std::ostream & os, ptr_t const & p ) {
      os << ( void * ) p.data();
      return os;
   }
}

#endif //TUDDBS_MEMORYDECORATOR_INCLUDE_MEMORY_TYPES_H

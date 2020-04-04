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
#ifndef TUDDBS_MEMORYDECORATOR_INCLUDE_UTILS_SINGLETON_H
#define TUDDBS_MEMORYDECORATOR_INCLUDE_UTILS_SINGLETON_H

namespace tuddbs {
   template< typename T >
   class singleton {
      public:
         static T* instance( ) {
            if( !_instance )
               _instance = new T( );
            return _instance;
         }

         singleton( singleton const & ) = delete;
         singleton & operator=( singleton const & ) = delete;

         virtual
         ~singleton( ) {
            _instance = 0;
         }
      protected:
         singleton( ) {}

      private:
         static T* _instance;
   };
   template< typename T >
   T * singleton< T >::_instance = 0;
}

#endif //TUDDBS_MEMORYDECORATOR_INCLUDE_UTILS_SINGLETON_H

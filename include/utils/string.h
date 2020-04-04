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
#ifndef TUDDBS_MEMORYDECORATOR_INCLUDE_UTILS_STRING_H
#define TUDDBS_MEMORYDECORATOR_INCLUDE_UTILS_STRING_H

#include <string>
#include <sstream>
#include <iostream>
namespace tuddbs {
   template< typename T >
   std::string toString( T value ) {
      std::ostringstream oss;
      oss << value;
      return oss.str();
   }

   std::string concat( std::initializer_list< std::string > strList ) {
      std::string ret = "";
      for( std::string s : strList ) {
         ret += s;
      }
      return ret;
   }
}

#endif //TUDDBS_MEMORYDECORATOR_INCLUDE_UTILS_STRING_H

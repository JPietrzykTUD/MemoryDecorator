/* 
 * This file is part of the SIMQ Project (https://github.com/JPietrzykTUD/SIMQ).
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
#ifndef TUDDBS_MEMORYDECORATOR_INCLUDE_UTILS_PREPROCESSOR_H
#define TUDDBS_MEMORYDECORATOR_INCLUDE_UTILS_PREPROCESSOR_H


#ifdef C17
#ifndef UNUSED
#define UNUSED [[maybe_unused]]
#endif
#else
#ifndef UNUSED
#define UNUSED __attribute__((unused))
#endif
#endif

#endif //TUDDBS_MEMORYDECORATOR_INCLUDE_UTILS_PREPROCESSOR_H

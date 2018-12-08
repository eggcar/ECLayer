/**
 * @file	ec_compiler_compat.h
 * @brief	Compiler compatable macro definitions for ECLayer.
 * @author	Eggcar
*/

/**
 * Copyright EggCar(eggcar@qq.com)
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * 	http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#ifndef __EC_COMPILER_COMPAT_H
#define __EC_COMPILER_COMPAT_H

#if defined(__GNUC__)
#	if defined(__GNUC_PATCHLEVEL__)
#		define __GNUC_VERSION__ (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#	else
#		define __GNUC_VERSION__ (__GNUC__ * 10000 + __GNUC_MINOR__ * 100)
#	endif
#endif

#ifdef __ICCARM__
#	define EC_ALIGN_ENTER(n) _Pragma("pack(push, n)")
#	define EC_ALIGN_STRUCT(n) ;
#	define EC_ALIGN_EXIT _Pragma("pack(pop)")
#elif defined __CC_ARM
#	if __ARMCC_VERSION > 6000000
#		define EC_ALIGN_ENTER(n) _Pragma("pack(push, n)")
#		define EC_ALIGN_STRUCT(n) ;
#		define EC_ALIGN_EXIT _Pragma("pack(pop)")
#	else
#		define EC_ALIGN_ENTER(n) ;
#		define EC_ALIGN_STRUCT(n) __attribute__((packed, aligned(n)))
#		define EC_ALIGN_EXIT
#	endif
#elif defined __GNUC__
#	define EC_ALIGN_ENTER(n) ;
#	define EC_ALIGN_STRUCT(n) __attribute__((packed, aligned(n)))
#	define EC_ALIGN_EXIT
#else
#	define EC_ALIGN_ENTER(n) ;
#	define EC_ALIGN_STRUCT(n) ;
#	define EC_ALIGN_EXIT
#endif

#ifdef __ICCARM__
#	define EC_OPTIMIZE_ENTER
#	define EC_OPTIMIZE_O0 _Pragma("optimize=none")
#	define EC_OPTIMIZE_O1 _Pragma("optimize=low")
#	define EC_OPTIMIZE_O2 _Pragma("optimize=medium")
#	define EC_OPTIMIZE_O3 _Pragma("optimize=high")
#	define EC_OPTIMIZE_SIZE _Pragma("optimize=size")
#	define EC_OPTIMIZE_DEBUG _Pragma("optimize=none")
#	define EC_OPTIMIZE_SPEED _Pragma("optimize=speed")
#	define EC_OPTIMIZE_EXIT
#elif defined __CC_ARM
#	if __ARMCC_VERSION > 6000000
/**
 * @warning Unfortunately, Armclang compiler doesn't support partial optimization.
 * 			God knows what are these guys thinking about...
*/
#		define EC_OPTIMIZE_ENTER
#		define EC_OPTIMIZE_O0
#		define EC_OPTIMIZE_O1
#		define EC_OPTIMIZE_O2
#		define EC_OPTIMIZE_O3
#		define EC_OPTIMIZE_SIZE
#		define EC_OPTIMIZE_DEBUG
#		define EC_OPTIMIZE_SPEED
#		define EC_OPTIMIZE_EXIT
#	else
#		define EC_OPTIMIZE_ENTER _Pragma("push")
#		define EC_OPTIMIZE_O0 _Pragma("O0")
#		define EC_OPTIMIZE_O1 _Pragma("O1")
#		define EC_OPTIMIZE_O2 _Pragma("O2")
#		define EC_OPTIMIZE_O3 _Pragma("O3")
#		define EC_OPTIMIZE_SIZE _Pragma("Ospace")
#		define EC_OPTIMIZE_DEBUG _Pramga("O0")
#		define EC_OPTIMIZE_SPEED _Pragma("Otime")
#		define EC_OPTIMIZE_EXIT _Pragma("pop")
#	endif
#elif defined __GNUC__
#	if __GNUC_VERSION__ > 40400
#		define EC_OPTIMIZE_ENTER _Pragma("GCC push_options")
#		define EC_OPTIMIZE_O0 _Pragma("GCC optimize(\"O0\")")
#		define EC_OPTIMIZE_O1 _Pragma("GCC optimize(\"O1\")")
#		define EC_OPTIMIZE_O2 _Pragma("GCC optimize(\"O2\")")
#		define EC_OPTIMIZE_O3 _Pragma("GCC optimize(\"O3\")")
#		define EC_OPTIMIZE_SIZE _Pragma("GCC optimze(\"Os\")")
#		define EC_OPTIMIZE_DEBUG _Pragma("GCC optimize(\"Og\")")
#		define EC_OPTIMIZE_SPEED _Pragma("GCC optimize(\"Ofast\")")
#		define EC_OPTIMIZE_EXIT _Pragma("GCC pop_options")
#	else
/**
 * @todo Check what to do with GCC/G++ version lower than 4.4
*/
#		define EC_OPTIMIZE_ENTER
#		define EC_OPTIMIZE_O0
#		define EC_OPTIMIZE_O1
#		define EC_OPTIMIZE_O2
#		define EC_OPTIMIZE_O3
#		define EC_OPTIMIZE_SIZE
#		define EC_OPTIMIZE_DEBUG
#		define EC_OPTIMIZE_SPEED
#		define EC_OPTIMIZE_EXIT
#	endif
#else
#	define EC_OPTIMIZE_ENTER
#	define EC_OPTIMIZE_O0
#	define EC_OPTIMIZE_O1
#	define EC_OPTIMIZE_O2
#	define EC_OPTIMIZE_O3
#	define EC_OPTIMIZE_SIZE
#	define EC_OPTIMIZE_DEBUG
#	define EC_OPTIMIZE_SPEED
#	define EC_OPTIMIZE_EXIT
#endif

#endif

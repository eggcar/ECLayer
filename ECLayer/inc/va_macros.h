/**
 * @file	va_macros.h
 * @brief	Support of variable argument macros.
 * @author	
*/

#pragma once

/**
 * To use this, define your macro like that below:
 * 
 * #define MyVaMacro(...) CONCAT2(MyVaMacro_, COUNT_ARGUMENTS(__VA_ARGS__))(__VA_ARGS__)
 * #define MyVaMacro_0(a) do_something
 * #define MyVaMacro_1(a) do_something_with(a)
 * #define MyVaMacro_2(a, b) do_something_with(a, b)
 * ......
*/

/**
 * Variable parameter macros assistant
 * See : https://renenyffenegger.ch/notes/development/languages/C-C-plus-plus/preprocessor/macros/__VA_ARGS__/count-arguments
*/
#ifndef ELEVENTH_ARGUMENT
#define ELEVENTH_ARGUMENT(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, ...) a11
#endif

/**
 * Variable parameter macros assistant
 * See : https://renenyffenegger.ch/notes/development/languages/C-C-plus-plus/preprocessor/macros/__VA_ARGS__/count-arguments
*/
#ifndef COUNT_ARGUMENTS
#define COUNT_ARGUMENTS(...) ELEVENTH_ARGUMENT(dummy, ## __VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#endif

/**
 * Variable parameter macros assistant
 * See : https://renenyffenegger.ch/notes/development/languages/C-C-plus-plus/preprocessor/macros/__VA_ARGS__/count-arguments
*/
#ifndef CONCAT
#define CONCAT(a, b) a ## b
#endif

/**
 * Variable parameter macros assistant
 * See : https://renenyffenegger.ch/notes/development/languages/C-C-plus-plus/preprocessor/macros/__VA_ARGS__/count-arguments
*/
#ifndef CONCAT2
#define CONCAT2(a, b) CONCAT(a, b)
#endif

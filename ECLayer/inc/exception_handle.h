/**
 *@file		exception_handle.h
 *@brief	Exception handle support for C language.
 *			Thanks to Francesco Nidito for this useful hack.
 *@author	Francesco Nidito. Modified by Eggcar
 */

/* Copyright (C) 2009-2015 Francesco Nidito 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions: 
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software. 
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 */

#ifndef _TRY_THROW_CATCH_H_
#define _TRY_THROW_CATCH_H_

#include <setjmp.h>
#include <stddef.h>

/* For the full documentation and explanation of the code below, please refer to
* http://www.di.unipi.it/~nids/docs/longjump_try_trow_catch.html
*/

#define TRY do { jmp_buf ex_buf__; switch( setjmp(ex_buf__) ) { case 0: while(1) {
#define CATCH(x) break; case x:
#define FINALLY break; } default: {
#define ENDTRY break; } } }while(0)
#define THROW(x) longjmp(ex_buf__, x)

#define CATCHES_EXCEPTION (ex_buf__)
#define THROWS_EXCEPTION jmp_buf exbuf__

/**
 * I tried to add two macros for functions that throw exceptions.
 * You can consider it as checked exception in Java (without exception
 * type specification), although its main purpose is to pass the long jump 
 * buffer from ex-catchers to ex-thowers.
 *
 * For each function that thows exception(s), you should declare and define
 * the function prototype like this :

			int32_t func_a(int32_t para1, ..., THROWS_EXCEPTION);

 * For callers of functions that thows exceptions, you must catch exceptions.
 * suggested code style :

		TRY {
			...;
			func_a(para1, ..., CATCHES_EXCEPTION);
			...;
			func_b(para2, ..., CATCHES_EXCEPTION);
			...;
		}
		CATCH(EX_XXX) {
			do_something();
		}
		CATCH(EX_YYY) {
			do_something_else();
		}
		...
		...
		FINALLY {
			release_resources();
		}
		ENDTRY;

 * The FINALLY block is optional.
 * With the benefit of the simplified checked exception mechanism, you're
 * forced to process exceptions, although it's not able to check if you have
 * dealed with all possible exception types might be thrown.
 * The exception descriptor is suggested to be manually managed in exceptions.h
 * or errno.h to avoid clash.
 * A dynamic distribution mechanism will be added later.
 */
/**
 * I found a simpler implementation below :
		#define TRY do { jmp_buf ex_buf__; int ex_rtn__; ex_rtn__ = setjmp(ex_buf__); if (ex_rtn__ == 0) 
		#define CATCH(x) else if (ex_rtn__ == x)
		#define CATCHALL else
		#define FINALLY
		#define ENDTRY } while(0)
 * This adds CATCHALL function that catches all other exceptions.
 * But I think this might be abused by someone that ignored important exceptions. 
 * So it's not in use now :)
 */


#endif /*!_TRY_THROW_CATCH_H_*/


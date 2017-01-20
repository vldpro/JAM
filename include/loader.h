#ifndef LOADER_H
#define LOADER_H

#include <stdio.h>
#include "vm.h"

/** 
* file structure:
* 	1. tag - 64 bit identifier
* 	2. version - 64 bit value
* 	3. string pool size in bytes ( 64 bit)
* 	4. functions count (64 bit )
* 	5. string pool - char sequence 
*	6. functions
*
* function structure:
*	1. name id
* 	2. arguments count 
*	3. commands count 
*	4. commands[commands count]
*/

typedef enum load_err_code_t {
	LOAD_OK = 0,
	ERR_IN_READING_HEADER,
	ERR_IN_READING_FUNCTION,
	ERR_IN_READING_CONST_POOL,
	ERR_INCORRECT_HEADER,
	ERR_IN_READING_FUNCTION_META

} load_err_code_t;


load_err_code_t
load_src_file( vm_t* vm, FILE* src );


#endif

#ifndef LOADER_H
#define LOADER_H

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

enum load_err_code {
	READ_OK,
	ERR_IN_READING_FUNCTION,
	ERR_INCORRECT_HEADER
} load_err_code_t;

char* get_load_err_code_msg( enum load_err_code_t const err ) { }


load_err_code_t
load_src_file( vm_t* vm, FILE* src );


#endif

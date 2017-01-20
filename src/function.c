#include "function.h"
#include <stdlib.h>

function_t* 
function_new( 
	size_t const args_count, 
	size_t const cmds_count, 
	char const * const cmds ) 
{
	/*function_t* func = malloc( sizeof(function_t) );
	*(func) = (function_t){ .args_count = args_count, .cmds_count = cmds_count, .cmds = cmds };

	return func; */
	return NULL;
}

void function_free( function_t* function ) { free( function-> cmds ); free( function ); }

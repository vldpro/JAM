#include "function.h"
#include <stdlib.h>

function_s* 
function_new( 
	size_t const args_count, 
	size_t const cmds_count, 
	char const * const cmds ) 
{
	function_s* func = malloc( sizeof(function_s) );
	*(func) = { .args_count = args_count, .cmds_count = cmds_count, .cmds = cmds };

	return func;
	
}

void function_free( function_s* func ) { free( function-> cmds ); free( function ); }

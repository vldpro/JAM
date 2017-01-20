#include "function.h"
#include <stdio.h>

void print_function_meta( function_t const * const func ) {
	puts("\n__function__");
	printf( "+-- Name id: %u \n", func-> name_id );
	printf( "+-- Args count: %u \n", func-> args_count );
	printf( "+-- Cmds count: %u \n", func-> cmds_count );
}

void print_function( function_t const * const  func ) {
	print_function_meta( func );

	printf( "    ____body____    \n" );
	for( size_t i = 0; i < func-> cmds_count; i++ )
		printf("       %i : %i\n", i, func-> cmds[i] );
}

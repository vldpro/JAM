#include "vm.h"

void vm_init_str_const_pool( struct str_pool* const pool, size_t const size ) {
	pool-> size = size;
	pool-> char_at = malloc( sizeof(char) * size );
	return;
}

void vm_free_str_const_pool( struct str_pool* pool ) {
	free( pool-> char_at );
}

void vm_init_functions( vm_t* const vm ) {
	vm-> functions = calloc( sizeof(function_t), vm-> funcs_count );
}

void vm_free_functions( vm_t* const vm ) {
	for( size_t i = 0; i < vm-> funcs_count; i++ )
		if( vm-> functions[i].cmds != NULL )
			free( vm-> functions[i].cmds );

	free( vm-> functions );
}

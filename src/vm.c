#include "vm.h"


void vm_init_str_const_pool( struct str_pool* const pool, size_t const size ) {
	pool-> size = size;
	pool-> char_at = malloc( sizeof(char) * size );
	return;
}

void vm_free_str_const_pool( struct str_pool* pool ) {
	free( pool-> char_at );
}

void vm_free_functions( vm_t* vm ) {
}

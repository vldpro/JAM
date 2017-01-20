#include "ref.h"
#include <stdlib.h>

ref_s* ref_new( size_t const size ) {
	ref_s* ref = malloc( sizeof(ref_s) );
	*(ref) = (ref_s){ .mem_size = size, .mem_ptr = malloc( size ) };

	return ref;
}

void ref_free( ref_s* ref ) { free( ref-> mem_ptr ); free( ref ); }

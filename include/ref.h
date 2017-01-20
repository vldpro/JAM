#ifndef REF_H
#define REF_H
#include <stdlib.h>

typedef struct ref {
	size_t mem_size;
	char* mem_ptr;
} ref_s;

ref_s* ref_new( size_t const size );

void ref_free( ref_s* ref );


#endif

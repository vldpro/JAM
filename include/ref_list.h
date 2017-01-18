#ifndef REFERENCE_LIST_H
#define REFERENCE_LIST_H

#include <stdlib.h>
#include <ref.h>

typedef struct ref {
	size_t mem_size;
	char* mem_ptr;
} ref_s;

typedef struct refs_list{
	size_t size;
	ref** refs;
} refs_list_s;



refs_list_s* refslist_new( size_t const size );

void refslist_free( refs_list_s* list );

size_t refslist_add( refs_list_s* list, ref_s* ref );

void refslist_remove( refs_list_s* list, size_t const ref_if );

#endif

#include "refs_list.h"

refs_list_s* refslist_new( size_t const size ) {
	refs_list_s* list = calloc( sizeof(refs_list_s), 1 );
	*(list) = (refs_list_s){ .size = size, .refs = malloc( sizeof(ref_s) * size ) };

	return list;
}

void refslist_free( refs_list_s* list ) { free( list-> refs ); free( list ); }

static size_t get_free_cell_idx( refs_list_s* list ) {
	for( size_t i = 0; i < list-> size; i++ ) 
		if( !list-> refs[i] ) return i; 

	return list-> size;
}

size_t refslist_add( refs_list_s* list, ref_s* ref ) {
	size_t free_cell_idx = get_free_cell_idx(list);

	if( free_cell_idx == list-> size ) return list-> size;
	list-> refs[ free_cell_idx ] = ref;	

	return free_cell_idx;
}

void refslist_remove( refs_list_s* list, size_t const ref_id ) {
	ref_free( list-> refs[ ref_id ] ); list-> refs[ ref_id ] = NULL;
}

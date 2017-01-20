#include <string.h>
#include <stdlib.h>
#include "util.h"

static inline size_t lines_count( char const * const char_seq, size_t const chars_count ) {
	size_t str_count = 0;

	for( size_t i = 0; i < chars_count; i++ ) if( char_seq[i] == '\0' ) str_count++;
	return str_count;
}

void split_by_lines( struct str_pool* const pool ) {
	pool-> str_count = lines_count( pool-> char_at, pool-> size );
	pool-> str_at = malloc( pool-> str_count * sizeof(char*) );
	pool-> str_at[0] = pool-> char_at;

	for( size_t i = 0, j = 1; i < pool-> size; i++ ) 
		if( pool-> char_at[i] == '\0' ) { 
			pool-> str_at[j++] = pool-> char_at + i + 1;
		}
}

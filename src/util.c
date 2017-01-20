#include <string.h>
#include <stdlib.h>

static inline size_t lines_count( char const * const char_seq ) {
	size_t const length = strlen( char_seq );
	size_t count = 0;

	for( size_t i = 0; i < length; i++ ) if( char_seq[i] == '\0' ) count++;
	return count;
}

void split_by_lines( char* char_seq, char** str_arr ) {
	size_t const length = strlen( char_seq );
	str_arr = malloc( lines_count( char_seq ) * sizeof(char*) );
	str_arr[0] = char_seq;

	for( size_t i = 0; i < length; i++ ) 
		if( char_seq[i] == '\0' ) str_arr[i] = char_seq + i + 1;
}

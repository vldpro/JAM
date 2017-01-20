#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdint.h>
#include <stdlib.h>

#pragma pack(push, 1)
typedef struct function {
	uint64_t name_id, args_count, cmds_count;
	char*  cmds;

} function_t;
#pragma pack(pop)

function_t* function_new( size_t const args_count, size_t const cmds_count, char const * const cmds );

void function_free( function_t* );

#endif

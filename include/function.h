#ifndef FUNCTION_H
#define FUNCTION_H

#pragma pack(push, 1)
struct function {
	uint64_t const name_id, args_count, cmds_count;

	char const * const cmds;

} function_s;
#pragma pack(pop)

function_s* function_new( size_t const args_count, size_t const cmds_count, char const * const cmds );

void function_free( function_s* );

#endif

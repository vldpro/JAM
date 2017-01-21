#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdint.h>
#include <stdlib.h>

//#pragma pack(push, 1)
typedef struct function {
	uint64_t name_id, args_count, cmds_count;
	char*  cmds;

} function_t;
//#pragma pack(pop)

#endif

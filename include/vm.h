#ifndef VM_H
#define VM_H

#include "refs_list.h"
#include "call_stack.h"
#include "constant_string_pool.h"
#include "function.h"

struct vm {
	struct str_pool {
		char* char_at;
		char** str_at;

		uint64_t size;
	} const_str_pool;

	uint64_t funcs_count;
	function_t* functions;

	refs_list_t * const refs_list;
	call_stack_t * const call_stack;
	
} vm_t;

#endif

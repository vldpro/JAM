#ifndef VM_H
#define VM_H

#include "refs_list.h"
#include "call_stack.h"
#include "constant_string_pool.h"
#include "function.h"

struct vm {
	struct str_pool {
		char* start;	
		char** by_idx;

		uint64_t size;
	} const_str_pool;

	struct val_pool {
		uint64_t* by_idx, size;
	} const_val_pool;

	function_s const * const functions;

	refs_list_s * const refs_list;
	call_stack_t * const call_stack;
	
} vm_t;

#endif

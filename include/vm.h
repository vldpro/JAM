#ifndef VM_H
#define VM_H

#include "refs_list.h"
#include "ctx_stack.h"
#include "function.h"

typedef struct vm {
	struct str_pool {
		char* char_at;
		char** str_at;

		uint64_t size;
	} const_str_pool;

	uint64_t funcs_count;
	function_t* functions;

	refs_list_s* refs_list;
	ctx_stack_t* ctx_stack;
	
} vm_t;

void vm_init_str_const_pool( struct str_pool* const pool, size_t const size );

void vm_free_str_const_pool( struct str_pool* pool );

void vm_free_functions( vm_t* vm );

#endif

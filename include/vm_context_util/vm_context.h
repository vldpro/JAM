#ifndef VM_CONTEXT_H
#define VM_CONTEXT_H

#include <stdlib.h>

typedef struct vm_context {
	stack_t* data_stack_ptr;
	stack_t eval_stack;	

	size_t instr_ptr;
	function_t const * const cur_func;

	struct vm_context* const prev_ctx;

} vm_context_t;


vm_context_t* vmctx_new( vm_context_t const ctx_data );

void vmctx_free( vm_context_t* ctx );

#endif

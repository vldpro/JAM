#include "ctx_stack.h"
#include <stdlib.h>
#include <stdio.h>

ctx_stack_t* 
ctx_stack_new(void) {
	return calloc( sizeof(ctx_stack_t), 1 );
}

void ctx_stack_free( ctx_stack_t* cstack ) {
	vm_context_t* cur_ctx = cstack-> cur_ctx;

	while( cur_ctx ) { 
		vm_context_t* to_clear = cur_ctx; 
		cur_ctx = cur_ctx-> prev_ctx; 
		vmctx_free( to_clear ); 
	}
}

void ctx_stack_push( ctx_stack_t* const cstack, vm_context_t* const ctx ) {
	ctx-> prev_ctx = cstack-> cur_ctx;
	cstack-> cur_ctx = ctx;
	cstack-> size++;
}

vm_context_t* ctx_stack_pop( ctx_stack_t* const cstack ) {
	vm_context_t* ctx = cstack-> cur_ctx;
	cstack-> cur_ctx = ctx-> prev_ctx;
	cstack-> size--;

	return ctx;
}

void ctx_stack_print_trace( ctx_stack_t const * const cstack, char** const name_at ) {
	vm_context_t* cur_ctx = cstack-> cur_ctx;
	if( cur_ctx == NULL ) return;
	size_t i = cstack-> size;

	do {
		printf("%08u: name: %s\n", i--, name_at[ cur_ctx-> cur_func-> name_id ]);
	} while( cur_ctx = cur_ctx-> prev_ctx );
}


#ifdef TEST_CTX_STACK

int main( int argc, char** argv ) {
	ctx_stack_t* ctx_stack = ctx_stack_new();

	vm_context_t* ctx = vmctx_new((vm_context_t) { .instr_ptr = 10 });
	ctx_stack_push( ctx_stack, ctx );	
	ctx = vmctx_new( (vm_context_t) { .instr_ptr = 257 });
	ctx_stack_push( ctx_stack, ctx );	

	return ctx-> instr_ptr;
} 

#endif

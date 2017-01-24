#include "vm_context.h"
#include "stack.h"

#include <stdlib.h>

vm_context_t* vmctx_new( vm_context_t const ctx_data ) {
	vm_context_t* ctx = calloc( sizeof(vm_context_t), 1 );	
	*ctx = ctx_data;

	return ctx;
}

void vmctx_free( vm_context_t* ctx ) { 
	stack_free( ctx-> eval_stack ); 
	stack_free( ctx-> local_data_stack );
	free( ctx ); 
}

#include "vm_context.h"
#include <stdlib.h>

vm_context_t* vmctx_new( vm_context_t const ctx_data ) {
	vm_context_t* ctx = malloc( sizeof(vm_context_t) );	
	*(ctx) = ctx_data;

	return ctx;
}

void vmctx_free( vm_context_t* ctx ) { }

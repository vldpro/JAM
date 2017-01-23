#ifndef CALL_STACK_H
#define CALL_STACK_H

#include "vm_context.h"
#include <stdlib.h>

typedef struct ctx_stack {
	size_t size;
	vm_context_t* cur_ctx;

} ctx_stack_t;

ctx_stack_t* ctx_stack_new(void);

void ctx_stack_free( ctx_stack_t* cstack );

void ctx_stack_push( ctx_stack_t* const cstack, vm_context_t * const ctx );

vm_context_t* ctx_stack_pop( ctx_stack_t* const cstack );

void ctx_stack_print_trace( ctx_stack_t const * const cstack, char** const name_at );

#endif

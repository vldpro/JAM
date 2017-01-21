#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdint.h>
#include "vm_errors.h"

#define ERR_TYPE enum vm_err_code

#define DEFAULT_EVAL_STACK_SIZE 25 

typedef struct stack {
	size_t size, sp;
	uint64_t* data;
} stack_t;

stack_t* stack_new( size_t const size );

int stack_is_full( stack_t const * const stack );

int stack_is_empty( stack_t const * const stack );

ERR_TYPE stack_push( stack_t* const stack, uint64_t const value );

ERR_TYPE stack_pop( stack_t* const stack, uint64_t * const var );

ERR_TYPE stack_peek( stack_t* const stack, uint64_t * const var );


void stack_free_data( stack_t* const stack );

void stack_clear( stack_t* const stack );

#endif

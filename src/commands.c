#include "commands.h"
#include "vm_errors.h"
#include "stack.h"
#include "vm_context.h"
#include "vm.h"
#include "ctx_stack.h"


#include <stdio.h>
#include <stdint.h>

#define TRY( action ) \
do{ int err = (action); if( (err) != OK ) return err; }while(0)


#define DEFINE_CMD( cmd_name ) \
enum vm_err_code cmd_##cmd_name( vm_t* const vm )

static inline stack_t* get_eval_stack( vm_t const * const vm ) {
	return vm-> ctx_stack-> cur_ctx-> eval_stack;
}

#define UNIPLEMENTED( cmd_name ) \
DEFINE_CMD(cmd_name) { return OK; }

#define DEFINE_BINOP( cmd_name, operation, type ) \
DEFINE_CMD(cmd_name) { \
	type a, b, result; \
	stack_t* const eval_stack = get_eval_stack( vm ); \
	TRY( stack_pop(eval_stack, (uint64_t*)&a) ); \
	TRY( stack_pop(eval_stack, (uint64_t*)&b) ); \
	result = a operation b; \
	stack_push(eval_stack, *((uint64_t*)&(result)) ); \
	\
	return OK; \
} 


DEFINE_BINOP(iadd, +, int64_t)
DEFINE_BINOP(dadd, +, double)

DEFINE_BINOP(isub, -, int64_t)
DEFINE_BINOP(dsub, -, double)

DEFINE_BINOP(imul, *, int64_t)
DEFINE_BINOP(dmul, *, double)

DEFINE_BINOP(idiv, /, int64_t)
DEFINE_BINOP(ddiv, /, double)

DEFINE_BINOP(and, &, uint64_t)
DEFINE_BINOP(or, |, uint64_t)
DEFINE_BINOP(xor, ^, uint64_t)
DEFINE_BINOP(mod, %, uint64_t)

#undef DEFINE_BINOP

DEFINE_CMD(invoke) {
	stack_t* const old_eval_stack = get_eval_stack( vm );
	uint64_t func_id;
	TRY( stack_pop(old_eval_stack, &func_id) );


	if( func_id >= vm-> funcs_count ) return NO_SUCH_FUNCTION;

	function_t* callee_func = vm-> functions + func_id;
	stack_t* new_eval_stack = stack_new( DEFAULT_EVAL_STACK_SIZE );

	// copy arguments
	for( size_t i = 0; i < callee_func-> args_count; i++ ) {
		size_t val;

		TRY( stack_pop( old_eval_stack, &val ) );
		stack_push( new_eval_stack, val );
	}

	// create new context
	ctx_stack_push(
		vm-> ctx_stack,
		vmctx_new( (vm_context_t) {
			.eval_stack = new_eval_stack,
			.instr_ptr = 0,
			.cur_func = callee_func
		})
	);

	return OK;
}

DEFINE_CMD(ret) {
	vm_context_t* executed_ctx = ctx_stack_pop( vm-> ctx_stack );
	uint64_t return_value;

	TRY( stack_pop( executed_ctx-> eval_stack, &return_value ) );
	TRY( stack_push( vm-> ctx_stack-> cur_ctx-> eval_stack, return_value ) );
	vmctx_free( executed_ctx );

	return OK;
}

DEFINE_CMD(nop) { return OK; }

DEFINE_CMD(iprint) {
	stack_t* const eval_stack = get_eval_stack( vm );
	int64_t val;
	TRY( stack_pop(eval_stack, (uint64_t*)&val) );

	printf( "%i", val );
}

DEFINE_CMD(dprint) {
	stack_t* const eval_stack = get_eval_stack( vm );
	double val;
	TRY( stack_pop(eval_stack, (uint64_t*)&val) );

	printf( "%d", val );

	return OK;
}

DEFINE_CMD(sprint) {
	stack_t* const eval_stack = get_eval_stack( vm );
	uint64_t str_id;
	TRY( stack_pop(eval_stack, &str_id) );

	char* to_print = vm-> const_str_pool.str_at[ str_id ];
	puts( to_print );

	return OK;
}

DEFINE_CMD(clear) {
	stack_t* const eval_stack = get_eval_stack( vm );
	stack_clear( eval_stack );

	return OK;
}

DEFINE_CMD(push) {
	vm_context_t* cur_ctx = vm-> ctx_stack-> cur_ctx;
	uint64_t constant = *((uint64_t*)(
		cur_ctx-> cur_func-> cmds + cur_ctx-> instr_ptr
	));

	cur_ctx-> instr_ptr += sizeof(uint64_t);

	TRY( stack_push( get_eval_stack(vm), constant ) );

	return OK;
}

DEFINE_CMD(dup) {
	uint64_t a;
	stack_t* const eval_stack = get_eval_stack( vm );

	TRY( stack_peek(eval_stack, &a) );
	TRY( stack_push(eval_stack, a) );

	return OK;
}

DEFINE_CMD(swap) {
	uint64_t a, b;
	stack_t* const eval_stack = get_eval_stack( vm );

	TRY( stack_pop(eval_stack, &a) );
	TRY( stack_pop(eval_stack, &b) );

	stack_push( eval_stack, a );
	stack_push( eval_stack, b );

	return OK;
}

DEFINE_CMD(branch) {
	uint64_t instr_ptr;
	stack_t* const eval_stack = get_eval_stack( vm );

	TRY( stack_pop(eval_stack, &instr_ptr) );
	vm-> ctx_stack-> cur_ctx-> instr_ptr = instr_ptr;

	return OK;
}

DEFINE_CMD(halt) { exit(0); }

DEFINE_CMD(branchif) {
	cmd_swap( vm );

	uint64_t compare_res;
	stack_pop( get_eval_stack(vm), &compare_res );

	if( compare_res ) cmd_branch( vm );
	return OK;
}

DEFINE_CMD(retvoid) {
	vmctx_free( ctx_stack_pop(vm-> ctx_stack) );
	return OK;
}

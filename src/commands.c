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

#define DEFINE_BINOP( cmd_name, operation, type, res_type ) \
DEFINE_CMD(cmd_name) { \
	type a, b; \
	\
	stack_t* const eval_stack = get_eval_stack( vm ); \
	TRY( stack_pop(eval_stack, (uint64_t*)&b) ); \
	TRY( stack_pop(eval_stack, (uint64_t*)&a) ); \
	\
	res_type result = a operation b; \
	TRY( stack_push(eval_stack, *((uint64_t*)&(result)) ) ); \
	\
	return OK; \
} 

#define DEFINE_COMPARE_OP( name, op, type ) \
DEFINE_CMD(name) { \
	type a, b; \
	\
	stack_t* const eval_stack = get_eval_stack( vm ); \
	TRY( stack_pop(eval_stack, (uint64_t*)&b) ); \
	TRY( stack_peek(eval_stack, (uint64_t*)&a) ); \
	stack_push( eval_stack, *((uint64_t*)&b) ); \
	\
	uint64_t cmp_result = a op b; \
	TRY( stack_push(eval_stack, *((uint64_t*)&(cmp_result))) ); \
	\
	return OK;\
}

#define DEFINE_MATH_OP( name, op, type ) \
	DEFINE_BINOP(name, op, type, type)


DEFINE_MATH_OP(iadd, +, int64_t)
DEFINE_MATH_OP(dadd, +, double)

DEFINE_MATH_OP(isub, -, int64_t)
DEFINE_MATH_OP(dsub, -, double)

DEFINE_MATH_OP(imul, *, int64_t)
DEFINE_MATH_OP(dmul, *, double)

DEFINE_MATH_OP(idiv, /, int64_t)
DEFINE_MATH_OP(ddiv, /, double)

DEFINE_MATH_OP(and, &, uint64_t)
DEFINE_MATH_OP(or, |, uint64_t)
DEFINE_MATH_OP(xor, ^, uint64_t)
DEFINE_MATH_OP(mod, %, uint64_t)

DEFINE_COMPARE_OP(icmpeq, ==, int64_t ); 
DEFINE_COMPARE_OP(dcmpeq, ==, double ); 

DEFINE_COMPARE_OP(icmpne, !=, int64_t ); 
DEFINE_COMPARE_OP(dcmpne, !=, double ); 

DEFINE_COMPARE_OP(icmpg, >, int64_t ); 
DEFINE_COMPARE_OP(dcmpg, >, double ); 

DEFINE_COMPARE_OP(icmpge, >=, int64_t ); 
DEFINE_COMPARE_OP(dcmpge, >=, double ); 

DEFINE_COMPARE_OP(icmpl, <, int64_t ); 
DEFINE_COMPARE_OP(dcmpl, <, double ); 

DEFINE_COMPARE_OP(icmple, <=, int64_t ); 
DEFINE_COMPARE_OP(dcmple, <=, double ); 

#undef DEFINE_COMPARE_OP
#undef DEFINE_MATH_OP
#undef DEFINE_BINOP

#define DEFINE_UNARY_OP( name, op, type ) \
DEFINE_CMD(name) { \
	stack_t* const eval_stack = get_eval_stack( vm ); \
	type val; \
	TRY( stack_pop(eval_stack, (uint64_t*)&val) ); \
	(val)op; \
	stack_push( eval_stack, *((uint64_t*)(&val)) ); \
		\
	return OK; \
}

DEFINE_UNARY_OP(dinc, ++, double)
DEFINE_UNARY_OP(iinc, ++, int64_t)

DEFINE_UNARY_OP(ddec, --, double)
DEFINE_UNARY_OP(idec, --, int64_t)

#undef DEFINE_UNARY_OP

DEFINE_CMD(neg) {
	stack_t* const eval_stack = get_eval_stack( vm );
	uint64_t val;
	TRY( stack_pop(eval_stack, &val) );

	val = -val;
	stack_push(eval_stack, val);
	
	return OK;
}

DEFINE_CMD(i2d) {
	stack_t* const eval_stack = get_eval_stack( vm );
	int64_t val;

	TRY( stack_pop(eval_stack, (uint64_t*)&val) );

	double val_as_double = (double) val;
	stack_push( eval_stack, *((uint64_t*)(&val_as_double)) );

	return OK;
}

DEFINE_CMD(d2i) {
	stack_t* const eval_stack = get_eval_stack( vm );
	double val;

	TRY( stack_pop(eval_stack, (uint64_t*)&val) );

	int64_t val_as_int = (int64_t) val;
	stack_push( eval_stack, *((uint64_t*)(&val_as_int)) );

	return OK;
}

#define DEFINE_LOAD_OP( name, src, dest, op ) \
DEFINE_CMD(name) { \
	stack_t* const eval_stack = get_eval_stack( vm ); \
	stack_t* const local_stack = vm-> ctx_stack-> cur_ctx-> local_data_stack; \
	uint64_t val; \
	\
	TRY( stack_##op(src, &val ) ); \
	TRY( stack_push(dest, val) ); \
	\
	return OK; \
}

DEFINE_LOAD_OP(load, local_stack, eval_stack, pop );
DEFINE_LOAD_OP(loadcp, local_stack, eval_stack, peek );

DEFINE_LOAD_OP(store, eval_stack, local_stack, pop );
DEFINE_LOAD_OP(storecp, eval_stack, local_stack, peek );

#undef DEFINE_LOAD_OP

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
			.local_data_stack = stack_new( DEFAULT_DATA_STACK_SIZE ),
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

	printf( "%li", (long)val );

	return OK;	
}

DEFINE_CMD(dprint) {
	stack_t* const eval_stack = get_eval_stack( vm );
	double val;
	TRY( stack_pop(eval_stack, (uint64_t*)&val) );

	printf( "%lf", val );

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

DEFINE_CMD(cprint) {
	stack_t* const eval_stack = get_eval_stack( vm );
	uint64_t ascii_code;
	TRY( stack_pop(eval_stack, &ascii_code) );

	printf("%c", (char)ascii_code);

	return OK;
}

#define DEFINE_CLEAR_OP( name, stack_name ) \
DEFINE_CMD(name) { \
	stack_t* const stack = vm-> ctx_stack-> cur_ctx-> stack_name; \
	stack_clear( stack ); \
	\
	return OK; \
}

DEFINE_CLEAR_OP( clrloc, local_data_stack );
DEFINE_CLEAR_OP( clreval, eval_stack );

#undef DEFINE_CLEAR_OP

DEFINE_CMD(pop) {
	stack_t* const eval_stack = get_eval_stack( vm );
	TRY( stack_delete_top( eval_stack ) );

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
	TRY( cmd_swap( vm ) );

	uint64_t compare_res;
	stack_t* eval_stack = get_eval_stack( vm );
	stack_pop( eval_stack, &compare_res );

	if( compare_res ) cmd_branch( vm );
	stack_delete_top( eval_stack );

	return OK;
}

DEFINE_CMD(retvoid) {
	vmctx_free( ctx_stack_pop(vm-> ctx_stack) );
	return OK;
}

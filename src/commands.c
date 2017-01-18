#include "commands.h"
#include "errors.h"
#include "stack.h"
#include "vm_context.h"
#include "vm_stack.h"
#include "vm.h"
#include <stdio.h>

#define FOR_BYTECODES(DO) \
	DO(iadd) \
	DO(dadd) \
	DO(isub) \
	DO(dsub) \

#define HANDLE_ERR( action ) \
do{ int err = (action); if( (err) != OK ) return err; }while(0)

#define DEFINE_CMD( cmd_name ) \
enum vm_err_code cmd_##cmd_name##( vm_t* const vm )

#define DEFINE_BINOP( cmd_name, operation, type ) \
DEFINE_CMD(cmd_name) { \
	type a, b; \
	stack_t* const eval_stack  \
		= &(vm-> ctx_stack-> cur_ctx-> eval_stack); \
	HANDLE_ERR( stack_pop(eval_stack, &a) ); \
	HANDLE_ERR( stack_pop(eval_stack, &b) ); \
	stack_push(eval_stack, a operation b); \
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

static inline stack_t* get_eval_stack( vm_t const * const vm ) {
	return &(vm-> ctx_stack-> cur_ctx-> eval_stack);
}

DEFINE_CMD(clear) {
	stack_t* const eval_stack = get_eval_stack( vm );

	stack_clear( eval_stack );

	return OK;
}

DEFINE_CMD(dup) {
	uint64_t a;
	stack_t* const eval_stack = get_eval_stack( vm );

	HANDLE_ERR( stack_peek(eval_stack, &a) );
	HANDLE_ERR( stack_push(eval_stack, a) );

	return OK;
}

DEFINE_CMD(swap) {
	uint64_t a, b;
	stack_t* const eval_stack = get_eval_stack( vm );

	HANDLE_ERR( stack_pop(eval_stack, &a) );
	HANDLE_ERR( stack_pop(eval_stack, &b) );

	stack_push( eval_stack, a );
	stack_push( eval_stack, b );

	return OK;
}

DEFINE_CMD(branch) {
	uint64_t instr_ptr;
	stack_t* const eval_stack = get_eval_stack( vm );

	HANDLE_ERR( stack_pop(eval_stack, &instr_ptr) );
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

#define DATA_STACK_SIZE 64

DEFINE_CMD(invoke) {//TODO }

DEFINE_CMD(retvoid) {
	vmctx_free( call_stack_pop(vm-> call_stack) );
	return OK;
}

DEFINE_CMD(ret) { //TODO }

typedef enum vm_err_code (*cmd_t) (vm_t*);

static inline char get_current_bytecode( vm_t const * const vm ) {
	vm_context_t const * const cur_ctx = vm-> ctx_stack-> cur_ctx;
	return cur_ctx-> cur_func-> cmds[ cur_ctx-> instr_ptr ];
}

static inline void err_handler( enum vm_err_code err, vm_t const * const vm,  FILE* errout ) {
	if( err == OK ) return;

	fprintf( errout, 
		"Error: %s ; Instr_ptr: %u ; Instr: %c", 
		get_err_msg(err), 
		vm-> ctx_stack-> cur_ctx-> instr_ptr, 
		get_current_bytecode(vm) );  

	exit(err);	
}


static void interpret( vm_t* const vm , FILE* errout ) {
	#define CMDS_ARRAY( name ) cmd_##name##,

	static const cmd_t cmd_list[] {
		FOR_BYTECODES( CMDS_ARRAY )
		0					
	}

	#undef CMDS_ARRAY

	//TODO

	for(;;) { err_handler( cmd_list[ get_current_bytecode(vm) ](vm), vm, errout );	}
}




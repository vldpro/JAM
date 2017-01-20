#include "commands.h"
#include "vm_errors.h"
#include "stack.h"
#include "vm_context.h"
#include "vm.h"
#include "ctx_stack.h"


#include <stdio.h>
#include <stdint.h>


#define FOR_BYTECODES(DO) \
	DO(nop) \
	DO(push) \
	DO(iadd) \
	DO(dadd) \
	DO(isub) \
	DO(dsub) \
	DO(idiv) \
	DO(ddiv) \
	DO(imul) \
	DO(dmul) \
	DO(mod) \
	DO(branch) \
	DO(branchif) \
	DO(invoke) \
	DO(retvoid) \
	DO(ret) \
	DO(dup) \
	DO(swap) \
	DO(iprint) \
	DO(dprint) \
	DO(sprint) \
	DO(halt) \

#define TRY( action ) \
do{ int err = (action); if( (err) != OK ) return err; }while(0)


#define DEFINE_CMD( cmd_name ) \
static enum vm_err_code cmd_##cmd_name( vm_t* const vm )

static inline stack_t* get_eval_stack( vm_t const * const vm ) {
	return vm-> ctx_stack-> cur_ctx-> eval_stack;
}

#define UNIPLEMENTED( cmd_name ) \
DEFINE_CMD(cmd_name) { return OK; }

#define DEFINE_BINOP( cmd_name, operation, type ) \
DEFINE_CMD(cmd_name) { \
	type a, b; \
	stack_t* const eval_stack = get_eval_stack( vm ); \
	TRY( stack_pop(eval_stack, (uint64_t*)&a) ); \
	TRY( stack_pop(eval_stack, (uint64_t*)&b) ); \
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
DEFINE_BINOP(mod, %, uint64_t)

#undef DEFINE_BINOP

UNIPLEMENTED(nop)
UNIPLEMENTED(iprint)
UNIPLEMENTED(dprint)
UNIPLEMENTED(sprint)
UNIPLEMENTED(ret)
UNIPLEMENTED(invoke)

DEFINE_CMD(clear) {
	stack_t* const eval_stack = get_eval_stack( vm );
	stack_clear( eval_stack );

	return OK;
}

DEFINE_CMD(push) {
	vm_context_t* cur_ctx = vm-> ctx_stack-> cur_ctx;
	uint64_t constant = *((uint64_t*)(cur_ctx-> cur_func-> cmds));

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

#define DATA_STACK_SIZE 64

DEFINE_CMD(retvoid) {
	vmctx_free( ctx_stack_pop(vm-> ctx_stack) );
	return OK;
}

typedef enum vm_err_code (*cmd_t) (vm_t*);

static inline char get_current_bytecode( vm_t const * const vm ) {
	vm_context_t const * const cur_ctx = vm-> ctx_stack-> cur_ctx;
	return cur_ctx-> cur_func-> cmds[ cur_ctx-> instr_ptr ];
}

static inline void err_handler( enum vm_err_code err, vm_t const * const vm,  FILE* errout ) {
	if( err == OK ) return;

	fprintf( errout, 
		"Error: %s ; Instr_ptr: %u ; Instr: %c", 
		"", 
		(unsigned int)vm-> ctx_stack-> cur_ctx-> instr_ptr, 
		get_current_bytecode(vm) 
		);  

	exit(err);	
}


void interpret( vm_t* const vm , FILE* const errout ) {
	#define CMDS_PTRS( name ) cmd_##name,

	static const cmd_t cmd_list[] = {
		FOR_BYTECODES( CMDS_PTRS )
		0					
	};

	#undef CMDS_PTRS

	//TODO

	for(;;) { err_handler( cmd_list[ get_current_bytecode(vm) ](vm), vm, errout );	}
}

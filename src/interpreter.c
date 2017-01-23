#include "interpreter.h"
#include "commands.h"
#include "vm_context.h"
#include "vm_errors.h"
#include "stack.h"

typedef enum vm_err_code (*cmd_t) (vm_t*);

static inline char get_current_bytecode( vm_t const * const vm ) {
	vm_context_t* const cur_ctx = vm-> ctx_stack-> cur_ctx;
	return cur_ctx-> cur_func-> cmds[ cur_ctx-> instr_ptr++ ];
}

static inline void err_handler( enum vm_err_code err, vm_t const * const vm, FILE* const errout ) {
	if( err == OK ) return;
	vm_context_t const * const cur_ctx = vm-> ctx_stack-> cur_ctx;


	puts("\n\n__ERROR__");
	fprintf( errout, " +-- func_name: %s\n", vm-> const_str_pool.str_at[ cur_ctx-> cur_func-> name_id ] );  
	fprintf( errout, " +-- addr: %08u\n", cur_ctx-> instr_ptr - 1 );
	fprintf( errout, " +-- Error code: %s\n", get_vm_err_msg(err) );

	#ifdef DEBUG
	puts( "\n EVAL STACK TRACE " );
	stack_print_trace( cur_ctx-> eval_stack );

	puts("\n DATA STACK TRACE " );
	stack_print_trace( cur_ctx-> local_data_stack );
	#endif
		

	exit(err);
}

void run( vm_t* const vm, FILE* const errout ) {
	#define CMD_PTRS( id, name ) cmd_##name,

	static const cmd_t cmd_ptrs[] = {
		FOR_BYTECODES( CMD_PTRS )
		0
	};

	#undef CMDS_PTRS

	for(;;) { err_handler( cmd_ptrs[ get_current_bytecode(vm) ](vm), vm, errout ); }
}


#include "interpreter.h"
#include "commands.h"
#include "vm_context.h"
#include "vm_errors.h"

typedef enum vm_err_code (*cmd_t) (vm_t*);

static inline char get_current_bytecode( vm_t const * const vm ) {
	vm_context_t* const cur_ctx = vm-> ctx_stack-> cur_ctx;
	return cur_ctx-> cur_func-> cmds[ cur_ctx-> instr_ptr++ ];
}

static inline void err_handler( enum vm_err_code err, vm_t const * const vm, FILE* const errout ) {
	if( err == OK ) return;
	vm_context_t const * const cur_ctx = vm-> ctx_stack-> cur_ctx;

	fprintf( errout, " func_name: %s", vm-> const_str_pool.str_at[ cur_ctx-> instr_ptr ] );  
	fprintf( errout, " addr: %08u", cur_ctx-> instr_ptr );
	fprintf( errout, " Error code: %i ", err );

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


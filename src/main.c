#include "loader.h"
#include "commands.h"
#include "vm.h"
#include "vm_context.h"
#include "ctx_stack.h"
#include "stack.h"

#include <stdio.h>

#define REFSLIST_SIZE 40
#define STACK_SIZE 60

int main( int argc, char** argv ) {
	if( argc <= 1 ) return 0;

	vm_t* vm = malloc( sizeof(vm_t) );	
	*vm = (vm_t){
		.ctx_stack = ctx_stack_new(),	
		.refs_list = refslist_new( REFSLIST_SIZE )
	};

	FILE* src_file = fopen( argv[1], "rb" );
	int err;
	if( err = load_src_file( vm, src_file ) ) return err;

	ctx_stack_push( 
		vm-> ctx_stack, 
		vmctx_new((vm_context_t) {
			.eval_stack = stack_new( STACK_SIZE ),
			.instr_ptr = 0,
			.cur_func = vm-> functions,
			.prev_ctx = NULL
		}) 
	);

	return 0;

	interpret( vm, stderr );
}

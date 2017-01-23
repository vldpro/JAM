#include "vm_errors.h"

char* get_vm_err_msg( enum vm_err_code err ) { 
	switch( err ) {
		case STACK_OF: return "STACK_OF";       
		case STACK_IS_EMPTY: return "STACK_IS_EMPTY";
		case INVALID_CMD_PTR: return "INVALID_CMD_PTR";
		case NO_SUCH_FUNCTION: return "NO_SUCH_FUNCTION";
		default: return "NO ERRCODE. BUG IN VM";
	}
}


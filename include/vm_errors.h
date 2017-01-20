#ifndef VM_ERRORS_H
#define VM_ERRORS_H

enum vm_err_code {
	OK,
	CALL_STACK_OF,
	STACK_OF,
	STACK_IS_EMPTY,
	INVALID_CMD_PTR

};

//char* get_vm_err_msg( enum vm_err_code err ); 

#endif

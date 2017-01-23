#pragma once

enum vm_err_code {
	OK = 0,
	STACK_OF,
	STACK_IS_EMPTY,
	INVALID_CMD_PTR,
	NO_SUCH_FUNCTION
};

char* get_vm_err_msg( enum vm_err_code err );


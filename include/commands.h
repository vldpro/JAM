#ifndef COMMANDS_H
#define COMMANDS_H

#include "vm_errors.h"
#include "vm.h"

#include <stdio.h>

void interpret( vm_t* const vm, FILE* const errout);

#endif

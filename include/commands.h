#ifndef COMMANDS_H
#define COMMANDS_H

#include "vm_errors.h"
#include "vm.h"

#include <stdio.h>

#define FOR_BYTECODES(DO) \
	DO(0, nop) \
	DO(1, push) \
	DO(2, iadd) \
	DO(3, dadd) \
	DO(4, isub) \
	DO(5, dsub) \
	DO(6, idiv) \
	DO(7, ddiv) \
	DO(8, imul) \
	DO(9, dmul) \
	DO(10, mod) \
	\
	DO(11, branch) \
	DO(12, branchif) \
	\
	DO(13, invoke) \
	DO(14, retvoid) \
	DO(15, ret) \
	DO(16, dup) \
	DO(17, swap) \
	\
	DO(18, iprint) \
	DO(19, dprint) \
	DO(20, sprint) \
	DO(21, halt) \
	\
	DO(22, icmpeq) \
	DO(23, icmpne) \
	DO(24, icmpg) \
	DO(25, icmpge) \
	DO(26, icmpl) \
	DO(27, icmple) \
	\
	DO(28, dcmpeq) \
	DO(29, dcmpne) \
	DO(30, dcmpg) \
	DO(31, dcmpge) \
	DO(32, dcmpl) \
	DO(33, dcmple) \
	\
	DO(34, neg) \
	DO(35, i2d) \
	DO(36, d2i) \
	\
	DO(37, load) \
	DO(38, store) \
	\
	DO(39, dinc) \
	DO(40, ddec) \
	DO(41, iinc) \
	DO(42, idec) \
	DO(43, clrloc) \
	DO(44, clreval) \
	DO(45, pop) \
	DO(46, loadcp ) \
	DO(47, storecp ) \
	DO(48, cprint) 


#define DEFINE_SIGNATURE( id, name )  \
enum vm_err_code cmd_##name( vm_t* const vm );

FOR_BYTECODES( DEFINE_SIGNATURE )

#endif


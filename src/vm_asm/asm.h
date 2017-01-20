#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>


#define writecmd(val) { char constant = (char)(val); fwrite(&constant, sizeof(char), 1, file ); }
#define writeconst(val, type) { type constant = (val); fwrite(&constant, sizeof(type), 1, file ); }
#define writestr(str) { char* to_write = str; fwrite( to_write, sizeof(char), strlen(to_write) + 1, file ); }

#define beg_program int main(int argc, char** argv) {
#define end_program }
#define translate_to( fname ) FILE* file = fopen( fname, "wb" );

#define def_head( tag, version ) \
	writeconst( tag, uint64_t ) \
	writeconst(version, uint64_t) 

	
#define def_const_pool( size_in_bytes ) \
	writeconst(size_in_bytes, uint64_t)
#define add_const_str(str) writestr(str)

#define def_function( name_id, args_count, locals_count, cmds_count ) \
	writeconst(name_id, uint64_t) \
	writeconst(args_count, uint64_t) \
	writeconst(locals_count, uint64_t) \
	writeconst(cmds_count, uint64_t) 

#define dpush(constant) \
	writecmd(1) \
	writeconst( constant, double ) 

#define ipush(constant) \
	writecmd(1)\
	writeconst( constant, uint64_t)

#define iadd writecmd(2) 
#define dadd writecmd(3) 
#define isub writecmd(4) 
#define dsub writecmd(5)
#define idiv writecmd(6)
#define ddiv writecmd(7)
#define imul writecmd(8)
#define dmul writecmd(9)
#define mod writecmd(10)
#define branch writecmd(11)
#define branchif writecmd(12)
#define invoke writecmd(13)
#define retvoid writecmd(14)
#define ret writecmd(15)
#define dup writecmd(16)
#define swap writecmd(17)
#define dprint writecmd(18)
#define iprint writecmd(19)
#define printstr writecmd(20)
#define halt writecmd(21)

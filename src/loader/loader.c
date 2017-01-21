#include "loader.h"
#include "function.h"
#include "vm.h"
#include "util.h"
#include <stdio.h>
#include "commands.h"


#define TRY_READ( action, err ) \
do { if( !(action) ) return err; } while(0)

#define TRY_EXEC( action ) \
do { size_t err = (action); if( err ) return err; } while(0)

#define TRY_WITH_FINALLY( action, finally ) \
do { size_t err = (action); if( err ) { (finally); return err; } } while(0)

struct file_header {
	uint64_t tag;
	uint64_t version;

	uint64_t str_pool_size;
	uint64_t functions_count;
};


#ifdef DEBUG
	
#define GET_MNEMONICS( id, cmd_name ) #cmd_name , 

static char* bc_mnemonics[256] = {
	FOR_BYTECODES(GET_MNEMONICS)
	0
};

void print_function( function_t const * const func ) {
	puts("\n\n__function_metadata__");	
	printf( "+-- Name id: %x \n", func-> name_id );
	printf( "+-- Args count: %u \n", func-> args_count );
	printf( "+-- Cmds count: %u \n", func-> cmds_count );

	puts("__bytecode_start__");
	puts(" addr | bytecode | mnemonic ");

	for( size_t i = 0; i < func-> cmds_count; i++ ) {
		printf(" %04i : %08i", i, func-> cmds[i] );
		printf(" : %s", bc_mnemonics[ func-> cmds[i] ] );
		if( func-> cmds[i] == 0x01) {
			i++;
			printf(" 0x%08x", *((uint64_t*)(func-> cmds + i)) );
			printf(" %u \n", *((uint64_t*)(func-> cmds + i)) );
			i+=7;
		} else puts("");
	}

	puts("__bytecode_end__");
}

void print_file_header( struct file_header header ) {
	printf("Tag: %u \n", header.tag );
	printf("Version: %u \n", header.version );
	printf("Pool size: %u \n", header.str_pool_size );
	printf("Funcs count: %u \n", header.functions_count);
}

void print_str_pool( char** pool, size_t const count ) {
	puts("___string_pool_start___");
	for( size_t i = 0; i < count; i++ ) puts(pool[i]);
	puts("___string_pool_end___");
}

#endif



static load_err_code_t
load_str_const_pool( struct str_pool* pool, FILE* src ) {
	TRY_READ( 
		fread(pool-> char_at, sizeof(char), pool-> size, src),
		ERR_IN_READING_CONST_POOL
		);

	split_by_lines( pool );

	#ifdef DEBUG 
	printf( "%u \n", pool-> str_count );
	print_str_pool( pool-> str_at, pool-> str_count );
	#endif

	return LOAD_OK;
}



static load_err_code_t
load_function( function_t* new_func, FILE* src ) {
	TRY_READ( 
		fread( new_func, sizeof(uint64_t), 3, src ),
		ERR_IN_READING_FUNCTION_META
		);
	
	TRY_READ( fread( 
		new_func-> cmds = malloc( sizeof(char) * new_func-> cmds_count ),  
		sizeof(char), 
		new_func-> cmds_count, 
		src ),
		ERR_IN_READING_FUNCTION
		);

	#ifdef DEBUG
	print_function( new_func );
	#endif

	return LOAD_OK;
}

static load_err_code_t
load_functions( function_t* functions, uint64_t const funcs_count, FILE* src ) { 
	for( size_t i = 0; i < funcs_count; i++ ) 
		TRY_EXEC( load_function(functions + i, src) );	

	return LOAD_OK;
}

static inline load_err_code_t
is_header_correct( struct file_header header ) {
	return  header.tag != 0xDEADDEADDEADDEAD || header.version != 1 ? ERR_INCORRECT_HEADER : LOAD_OK;
}

load_err_code_t
load_src_file( vm_t* vm, FILE* src ) {
	struct file_header header = {};
	TRY_READ( 
		fread(&header, sizeof(struct file_header), 1, src), 
		ERR_IN_READING_HEADER
	);


	#ifdef DEBUG
	print_file_header( header );
	#endif

	TRY_EXEC( is_header_correct(header) );

	vm_init_str_const_pool( &(vm-> const_str_pool), header.str_pool_size );

	TRY_WITH_FINALLY( 
		load_str_const_pool( &(vm-> const_str_pool), src ),
		vm_free_str_const_pool( &(vm-> const_str_pool) )
		);

	vm-> functions = malloc( sizeof(function_t) * header.functions_count );
	vm-> funcs_count = header.functions_count;

	TRY_WITH_FINALLY( 
		load_functions( vm-> functions, vm-> funcs_count, src ),
		{ 
		vm_free_str_const_pool( &(vm-> const_str_pool) ); 
		vm_free_functions( vm ); 
		}
		);

	return LOAD_OK;
}


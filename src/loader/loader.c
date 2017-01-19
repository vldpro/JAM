#include "loader.h"
#include "function.c"
#include "vm.h"
#include "util.h"

#include <stdio.h>


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


static load_err_code_t
load_str_const_pool( struct str_pool* pool, FILE* src ) {
	TRY_READ( 
		fread(pool-> char_at, sizeof(char), pool-> size, src),
		ERR_IN_READING_CONST_POOL
		);

	split_by_lines( pool-> char_at, pool-> str_at ); //unimplimented

	return LOAD_OK;
}

static inline load_err_code_t
check_header( struct file_header header ) {
	return  header.tag != 0xDEADDEADDEADDEAD || header.version != 1 ? ERR_INCORRENCT_HEADER : LOAD_OK;
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

	return LOAD_OK;
}

static load_err_code_t
load_functions( function_t* functions, uint64_t const functions_count, FILE* src ) { 
	for( size_t i = 0; i < funcs_count; i++ ) 
		TRY_EXEC( load_function(function + i, src) );	

	return LOAD_OK;
}


load_err_code_t
load_src_file( vm_t* vm, FILE* src ) {
	struct file_header header = {};
	TRY_EXEC( fread(&header, sizeof(struct file_header), 1, src) );

	vm_init_str_const_pool( &(vm-> const_str_pool), header-> str_pool_size );

	TRY_WITH_FINALLY( 
		load_str_const_pool( &(vm-> const_str_pool), src ),
		vm_free_const_pool( &(vm-> const_str_pool), src )
		);

	vm-> functions = malloc( sizeof(function_t) * header-> functions_count );

	TRY_WITH_FINALLY( 
		load_functions( vm-> functions, vm-> funcs_count, src ),
		{ 
		vm_free_const_pool( &(vm-> const_str_pool) ); 
		vm_free_functions( vm ); 
		}
		);

	return LOAD_OK;
}




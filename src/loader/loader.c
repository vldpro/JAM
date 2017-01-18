#include "loader.h"
#include "function.c"
#include "vm.h"
#include "util.h"

#include <stdio.h>


#define HANDLE_ERR( action ) \
do { if( !(action) ) return ERR_IN_READING_FUNCTION; } while(0)

#define THROW_ERR( action ) \
do { size_t err = (action); if( err ) return err; } while(0)

struct file_header {
	uint64_t tag;
	uint64_t version;

	uint64_t str_pool_size;
	uint64_t str_pool_offset;

	uint64_t val_pool_size;
	uint64_t val_pool_offset;

	uint64_t functions_count;
	uint64_t functions_offset;
};


static load_err_code_t
load_str_const_pool( struct str_pool* pool, FILE* src ) {
	char* chars = malloc( sizeof(char) * pool-> size );	
	HANDLE_ERR( fread(chars, sizeof(char), pool-> size, src) );

	split_by_lines( chars, pool-> by_idx ); //unimplimented

	return READ_OK;
}

static load_err_code_t
load_val_const_pool( struct val_pool* pool, FILE* src ) {
	pool-> by_idx = malloc( sizeof(uint64_t) * pool-> size );
	HANDLE_ERR( fread(pool-> by_idx, sizeof(uint64_t), pool-> size, src) );

	return READ_OK;
}

static inline load_err_code_t
check_header( struct file_header header ) {
	return  header.tag != 0xDEADDEADDEADDEAD || header.version != 1 ? ERR_INCORRENCT_HEADER : READ_OK;
}

static load_err_code_t
load_function( function_t* new_func, FILE* src ) {

	HANDLE_ERR( fread( new_func, sizeof(uint64_t), 3, src ) );
	
	HANDLE_ERR( fread( 
		new_func.cmds = malloc( sizeof(char) * new_func-> cmds_count ),  
		sizeof(char), 
		new_func-> cmds_count, 
		src ) );

	return READ_OK;
}

static load_err_code_t
load_functions( function_t** functions, FILE* src ) { 
	uint64_t funcs_count;
	HANDLE_ERR( fread( &funcs_count, sizeof(struct funcs_header), 1, src ) );
	functions = malloc( sizeof(function_t*) * funcs_count );

	for( int i = 0; i < funcs_count; i++ ) 
		THROW_ERR( load_function(function[i], src) );	

	return READ_OK;
}

load_err_code_t
load_src_file( vm_t* vm, FILE* src ) {
	struct file_header header = {};
	THROW_ERR( fread(&header, sizeof(struct file_header), 1, src) );

	load_functions( &(vm-> functions), src );
	//TODO
	return READ_OK;
}



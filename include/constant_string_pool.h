#ifndef CONSTANT_STRING_POOL
#define CONSTANT_STRING_POOL

struct const_str_pool {
	size_t str_count;
	char* start_ptr;
	char** strings;
};

#endif

PHPAPI void php_register_variable_ex(char *var_name, zval *val, zval *track_vars_array TSRMLS_DC)
{
	char *p = NULL;
	char *ip = NULL;		 
	char *index;
	char *var, *var_orig;
	int var_len, index_len;
	zval *gpc_element, **gpc_element_p;
	zend_bool is_array = 0;
	HashTable *symtable1 = NULL;
	ALLOCA_FLAG(use_heap)

	assert(var_name != NULL);

	if (track_vars_array) {
		symtable1 = Z_ARRVAL_P(track_vars_array);
	}

	if (!symtable1) {
		 
		zval_dtor(val);
		return;
	}


	 
	while (*var_name && *var_name==' ') {
		var_name++;
	}

	 
	var_len = strlen(var_name);
	var = var_orig = do_alloca(var_len + 1, use_heap);
	memcpy(var_orig, var_name, var_len + 1);

	 
	for (p = var; *p; p++) {
		if (*p == ' ' || *p == '.') {
			*p='_';
		} else if (*p == '[') {
			is_array = 1;
			ip = p;
			*p = 0;
			break;
		}
	}
	var_len = p - var;

	if (var_len==0) {  
		zval_dtor(val);
		free_alloca(var_orig, use_heap);
		return;
	}

	 
	if (symtable1 == EG(active_symbol_table) &&
		var_len == sizeof("GLOBALS")-1 &&
		!memcmp(var, "GLOBALS", sizeof("GLOBALS")-1)) {
		zval_dtor(val);
		free_alloca(var_orig, use_heap);
		return;
	}

	index = var;
	index_len = var_len;

	if (is_array) {
		int nest_level = 0;
		while (1) {
			char *index_s;
			int new_idx_len = 0;

			if(++nest_level > PG(max_input_nesting_level)) {
				HashTable *ht;
				 

				if (track_vars_array) {
					ht = Z_ARRVAL_P(track_vars_array);
					zend_symtable_del(ht, var, var_len + 1);
				}

				zval_dtor(val);

				 
				if (!PG(display_errors)) {
					php_error_docref(NULL TSRMLS_CC, E_WARNING, "Input variable nesting level exceeded %ld. To increase the limit change max_input_nesting_level in php.ini.", PG(max_input_nesting_level));
				}
				free_alloca(var_orig, use_heap);
				return;
			}

			ip++;
			index_s = ip;
			if (isspace(*ip)) {
				ip++;
			}
			if (*ip==']') {
				index_s = NULL;
			} else {
				ip = strchr(ip, ']');
				if (!ip) {
					 
					*(index_s - 1) = '_';

					index_len = 0;
					if (index) {
						index_len = strlen(index);
					}
					goto plain_var;
					return;
				}
				*ip = 0;
				new_idx_len = strlen(index_s);
			}

			if (!index) {
				MAKE_STD_ZVAL(gpc_element);
				array_init(gpc_element);
				if (zend_hash_next_index_insert(symtable1, &gpc_element, sizeof(zval *), (void **) &gpc_element_p) == FAILURE) {
					zval_ptr_dtor(&gpc_element);
					zval_dtor(val);
					free_alloca(var_orig, use_heap);
					return;
				}
			} else {
				if (zend_symtable_find(symtable1, index, index_len + 1, (void **) &gpc_element_p) == FAILURE
					|| Z_TYPE_PP(gpc_element_p) != IS_ARRAY) {
					MAKE_STD_ZVAL(gpc_element);
					array_init(gpc_element);
					zend_symtable_update(symtable1, index, index_len + 1, &gpc_element, sizeof(zval *), (void **) &gpc_element_p);
				}
			}
			symtable1 = Z_ARRVAL_PP(gpc_element_p);
			 
			index = index_s;
			index_len = new_idx_len;

			ip++;
			if (*ip == '[') {
				is_array = 1;
				*ip = 0;
			} else {
				goto plain_var;
			}
		}
	} else {
plain_var:
		MAKE_STD_ZVAL(gpc_element);
		gpc_element->value = val->value;
		Z_TYPE_P(gpc_element) = Z_TYPE_P(val);
		if (!index) {
			if (zend_hash_next_index_insert(symtable1, &gpc_element, sizeof(zval *), (void **) &gpc_element_p) == FAILURE) {
				zval_ptr_dtor(&gpc_element);
			}
		} else {
			 
			if (PG(http_globals)[TRACK_VARS_COOKIE] &&
				symtable1 == Z_ARRVAL_P(PG(http_globals)[TRACK_VARS_COOKIE]) &&
				zend_symtable_exists(symtable1, index, index_len + 1)) {
				zval_ptr_dtor(&gpc_element);
			} else {
				zend_symtable_update(symtable1, index, index_len + 1, &gpc_element, sizeof(zval *), (void **) &gpc_element_p);
			}
		}
	}
	free_alloca(var_orig, use_heap);
}
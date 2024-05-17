PHP_METHOD(SoapClient, __call)
{
	char *function, *location=NULL, *soap_action = NULL, *uri = NULL;
	int function_len, i = 0;
	HashTable* soap_headers = NULL;
	zval *options = NULL;
	zval *headers = NULL;
	zval *output_headers = NULL;
	zval *args;
	zval **real_args = NULL;
	zval **param;
	int arg_count;
	zval **tmp;
	zend_bool free_soap_headers = 0;

	HashPosition pos;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sa|a!zz",
		&function, &function_len, &args, &options, &headers, &output_headers) == FAILURE) {
		return;
	}

	if (options) {
		HashTable *hto = Z_ARRVAL_P(options);
		if (zend_hash_find(hto, "location", sizeof("location"), (void**)&tmp) == SUCCESS &&
			Z_TYPE_PP(tmp) == IS_STRING) {
			location = Z_STRVAL_PP(tmp);
		}

		if (zend_hash_find(hto, "soapaction", sizeof("soapaction"), (void**)&tmp) == SUCCESS &&
			Z_TYPE_PP(tmp) == IS_STRING) {
			soap_action = Z_STRVAL_PP(tmp);
		}

		if (zend_hash_find(hto, "uri", sizeof("uri"), (void**)&tmp) == SUCCESS &&
			Z_TYPE_PP(tmp) == IS_STRING) {
			uri = Z_STRVAL_PP(tmp);
		}
	}

	if (headers == NULL || Z_TYPE_P(headers) == IS_NULL) {
	} else if (Z_TYPE_P(headers) == IS_ARRAY) {
		soap_headers = Z_ARRVAL_P(headers);
		verify_soap_headers_array(soap_headers TSRMLS_CC);
		free_soap_headers = 0;
	} else if (Z_TYPE_P(headers) == IS_OBJECT &&
	           instanceof_function(Z_OBJCE_P(headers), soap_header_class_entry TSRMLS_CC)) {
	    soap_headers = emalloc(sizeof(HashTable));
		zend_hash_init(soap_headers, 0, NULL, ZVAL_PTR_DTOR, 0);
		zend_hash_next_index_insert(soap_headers, &headers, sizeof(zval*), NULL);
		Z_ADDREF_P(headers);
		free_soap_headers = 1;
	} else{
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid SOAP header");
		return;
	}

	 
	if (zend_hash_find(Z_OBJPROP_P(this_ptr), "__default_headers", sizeof("__default_headers"), (void **) &tmp)==SUCCESS &&
	    Z_TYPE_PP(tmp) == IS_ARRAY) {
		HashTable *default_headers = Z_ARRVAL_P(*tmp);
		if (soap_headers) {
			if (!free_soap_headers) {
				HashTable *t =  emalloc(sizeof(HashTable));
				zend_hash_init(t, 0, NULL, ZVAL_PTR_DTOR, 0);
				zend_hash_copy(t, soap_headers, (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *));
				soap_headers = t;
				free_soap_headers = 1;
			}
			zend_hash_internal_pointer_reset(default_headers);
			while (zend_hash_get_current_data(default_headers, (void**)&tmp) == SUCCESS) {
				Z_ADDREF_PP(tmp);
				zend_hash_next_index_insert(soap_headers, tmp, sizeof(zval *), NULL);
				zend_hash_move_forward(default_headers);
			}
		} else {
			soap_headers = Z_ARRVAL_P(*tmp);
			free_soap_headers = 0;
		}
	}
	
	arg_count = zend_hash_num_elements(Z_ARRVAL_P(args));

	if (arg_count > 0) {
		real_args = safe_emalloc(sizeof(zval *), arg_count, 0);
		for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(args), &pos);
			zend_hash_get_current_data_ex(Z_ARRVAL_P(args), (void **) &param, &pos) == SUCCESS;
			zend_hash_move_forward_ex(Z_ARRVAL_P(args), &pos)) {
				 
				real_args[i++] = *param;
		}
	}
	if (output_headers) {
		array_init(output_headers);
	}
	do_soap_call(this_ptr, function, function_len, arg_count, real_args, return_value, location, soap_action, uri, soap_headers, output_headers TSRMLS_CC);
	if (arg_count > 0) {
		efree(real_args);
	}

	if (soap_headers && free_soap_headers) {
		zend_hash_destroy(soap_headers);
		efree(soap_headers);
	}
}

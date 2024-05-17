PHP_METHOD(SoapFault, SoapFault)
{
	char *fault_string = NULL, *fault_code = NULL, *fault_actor = NULL, *name = NULL, *fault_code_ns = NULL;
	int fault_string_len, fault_actor_len = 0, name_len = 0, fault_code_len = 0;
	zval *code = NULL, *details = NULL, *headerfault = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zs|s!z!s!z",
		&code,
		&fault_string, &fault_string_len,
		&fault_actor, &fault_actor_len,
		&details, &name, &name_len, &headerfault) == FAILURE) {
		return;
	}

	if (Z_TYPE_P(code) == IS_NULL) {
	} else if (Z_TYPE_P(code) == IS_STRING) {
		fault_code = Z_STRVAL_P(code);
		fault_code_len = Z_STRLEN_P(code);
	} else if (Z_TYPE_P(code) == IS_ARRAY && zend_hash_num_elements(Z_ARRVAL_P(code)) == 2) {
		zval **t_ns, **t_code;

		zend_hash_internal_pointer_reset(Z_ARRVAL_P(code));
		zend_hash_get_current_data(Z_ARRVAL_P(code), (void**)&t_ns);
		zend_hash_move_forward(Z_ARRVAL_P(code));
		zend_hash_get_current_data(Z_ARRVAL_P(code), (void**)&t_code);
		if (Z_TYPE_PP(t_ns) == IS_STRING && Z_TYPE_PP(t_code) == IS_STRING) {
		  fault_code_ns = Z_STRVAL_PP(t_ns);
		  fault_code = Z_STRVAL_PP(t_code);
		  fault_code_len = Z_STRLEN_PP(t_code);
		} else {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid fault code");
			return;
		}
	} else  {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid fault code");
		return;
	}
	if (fault_code != NULL && fault_code_len == 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid fault code");
		return;
	}
	if (name != NULL && name_len == 0) {
		name = NULL;
	}

	set_soap_fault(this_ptr, fault_code_ns, fault_code, fault_string, fault_actor, details, name TSRMLS_CC);
	if (headerfault != NULL) {
		add_property_zval(this_ptr, "headerfault", headerfault);
	}
}

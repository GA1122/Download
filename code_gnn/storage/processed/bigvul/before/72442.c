static int register_bound_param(INTERNAL_FUNCTION_PARAMETERS, pdo_stmt_t *stmt, int is_param)  
{
	struct pdo_bound_param_data param = {0};
	long param_type = PDO_PARAM_STR;

	param.paramno = -1;

	if (FAILURE == zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC,
			"lz|llz!", &param.paramno, &param.parameter, &param_type, &param.max_value_len,
			&param.driver_params)) {
		if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz|llz!", &param.name,
				&param.namelen, &param.parameter, &param_type, &param.max_value_len,
				&param.driver_params)) {
			return 0;
		}
	}

	param.param_type = (int) param_type;

	if (param.paramno > 0) {
		--param.paramno;  
	} else if (!param.name) {
		pdo_raise_impl_error(stmt->dbh, stmt, "HY093", "Columns/Parameters are 1-based" TSRMLS_CC);
		return 0;
	}

	Z_ADDREF_P(param.parameter);
	if (!really_register_bound_param(&param, stmt, is_param TSRMLS_CC)) {
		if (param.parameter) {
			zval_ptr_dtor(&(param.parameter));
			param.parameter = NULL;
		}
		return 0;
	}
	return 1;
}  
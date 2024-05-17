static PHP_METHOD(PDOStatement, bindValue)
{
	struct pdo_bound_param_data param = {0};
	long param_type = PDO_PARAM_STR;
	PHP_STMT_GET_OBJ;

	param.paramno = -1;

	if (FAILURE == zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC,
			"lz/|l", &param.paramno, &param.parameter, &param_type)) {
		if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz/|l", &param.name,
				&param.namelen, &param.parameter, &param_type)) {
			RETURN_FALSE;
		}
	}

	param.param_type = (int) param_type;

	if (param.paramno > 0) {
		--param.paramno;  
	} else if (!param.name) {
		pdo_raise_impl_error(stmt->dbh, stmt, "HY093", "Columns/Parameters are 1-based" TSRMLS_CC);
		RETURN_FALSE;
	}

	Z_ADDREF_P(param.parameter);
	if (!really_register_bound_param(&param, stmt, TRUE TSRMLS_CC)) {
		if (param.parameter) {
			zval_ptr_dtor(&(param.parameter));
			param.parameter = NULL;
		}
		RETURN_FALSE;
	}
	RETURN_TRUE;
}

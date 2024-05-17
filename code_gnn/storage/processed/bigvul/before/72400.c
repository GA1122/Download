static PHP_METHOD(PDOStatement, getColumnMeta)
{
	long colno;
	struct pdo_column_data *col;
	PHP_STMT_GET_OBJ;

	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &colno)) {
		RETURN_FALSE;
	}
	if(colno < 0) {
		pdo_raise_impl_error(stmt->dbh, stmt, "42P10", "column number must be non-negative" TSRMLS_CC);
		RETURN_FALSE;
	}

	if (!stmt->methods->get_column_meta) {
		pdo_raise_impl_error(stmt->dbh, stmt, "IM001", "driver doesn't support meta data" TSRMLS_CC);
		RETURN_FALSE;
	}

	PDO_STMT_CLEAR_ERR();
	if (FAILURE == stmt->methods->get_column_meta(stmt, colno, return_value TSRMLS_CC)) {
		PDO_HANDLE_STMT_ERR();
		RETURN_FALSE;
	}

	 
	col = &stmt->columns[colno];
	add_assoc_string(return_value, "name", col->name, 1);
	add_assoc_long(return_value, "len", col->maxlen);  
	add_assoc_long(return_value, "precision", col->precision);
	if (col->param_type != PDO_PARAM_ZVAL) {
		 
		add_assoc_long(return_value, "pdo_type", col->param_type);
	}
}

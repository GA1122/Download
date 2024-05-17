static PHP_METHOD(PDOStatement, errorInfo)
{
	int error_count;
	int error_count_diff     = 0;
	int error_expected_count = 3;

	PHP_STMT_GET_OBJ;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	array_init(return_value);
	add_next_index_string(return_value, stmt->error_code, 1);

	if (stmt->dbh->methods->fetch_err) {
		stmt->dbh->methods->fetch_err(stmt->dbh, stmt, return_value TSRMLS_CC);
	}

	error_count = zend_hash_num_elements(Z_ARRVAL_P(return_value));

	if (error_expected_count > error_count) {
		int current_index;

		error_count_diff = error_expected_count - error_count;
		for (current_index = 0; current_index < error_count_diff; current_index++) {
			add_next_index_null(return_value);
		}
	}
}

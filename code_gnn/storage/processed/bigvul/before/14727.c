PHP_FUNCTION(pg_result_seek)
{
	zval *result;
	long row;
	pgsql_result_handle *pg_result;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &result, &row) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, &result, -1, "PostgreSQL result", le_result);

	if (row < 0 || row >= PQntuples(pg_result->result)) {
		RETURN_FALSE;
	}

	 
	pg_result->row = row;
	RETURN_TRUE;
}
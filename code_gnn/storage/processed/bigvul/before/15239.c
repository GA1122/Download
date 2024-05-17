static void php_pgsql_get_result_info(INTERNAL_FUNCTION_PARAMETERS, int entry_type)
{
	zval *result;
	PGresult *pgsql_result;
	pgsql_result_handle *pg_result;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &result) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, &result, -1, "PostgreSQL result", le_result);

	pgsql_result = pg_result->result;

	switch (entry_type) {
		case PHP_PG_NUM_ROWS:
			Z_LVAL_P(return_value) = PQntuples(pgsql_result);
			break;
		case PHP_PG_NUM_FIELDS:
			Z_LVAL_P(return_value) = PQnfields(pgsql_result);
			break;
		case PHP_PG_CMD_TUPLES:
#if HAVE_PQCMDTUPLES
			Z_LVAL_P(return_value) = atoi(PQcmdTuples(pgsql_result));
#else
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Not supported under this build");
			Z_LVAL_P(return_value) = 0;
#endif
			break;
		default:
			RETURN_FALSE;
	}
	Z_TYPE_P(return_value) = IS_LONG;
}

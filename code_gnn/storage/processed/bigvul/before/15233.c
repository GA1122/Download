static void php_pgsql_do_async(INTERNAL_FUNCTION_PARAMETERS, int entry_type) 
{
	zval *pgsql_link;
	int id = -1;
	PGconn *pgsql;
	PGresult *pgsql_result;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "r",
								 &pgsql_link) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, &pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	if (PQ_SETNONBLOCKING(pgsql, 1)) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Cannot set connection to nonblocking mode");
		RETURN_FALSE;
	}
	switch(entry_type) {
		case PHP_PG_ASYNC_IS_BUSY:
			PQconsumeInput(pgsql);
			Z_LVAL_P(return_value) = PQisBusy(pgsql);
			Z_TYPE_P(return_value) = IS_LONG;
			break;
		case PHP_PG_ASYNC_REQUEST_CANCEL:
			Z_LVAL_P(return_value) = PQrequestCancel(pgsql);
			Z_TYPE_P(return_value) = IS_LONG;
			while ((pgsql_result = PQgetResult(pgsql))) {
				PQclear(pgsql_result);
			}
			break;
		default:
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "PostgreSQL module error, please report this error");
			break;
	}
	if (PQ_SETNONBLOCKING(pgsql, 0)) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Cannot set connection to blocking mode");
	}
	convert_to_boolean_ex(&return_value);
}

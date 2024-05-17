static void php_pgsql_get_field_info(INTERNAL_FUNCTION_PARAMETERS, int entry_type)
{
	zval *result;
	long field;
	PGresult *pgsql_result;
	pgsql_result_handle *pg_result;
	Oid oid;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &result, &field) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, &result, -1, "PostgreSQL result", le_result);

	pgsql_result = pg_result->result;
	
	if (field < 0 || field >= PQnfields(pgsql_result)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Bad field offset specified");
		RETURN_FALSE;
	}

	switch (entry_type) {
		case PHP_PG_FIELD_NAME:
			Z_STRVAL_P(return_value) = PQfname(pgsql_result, field);
			Z_STRLEN_P(return_value) = strlen(Z_STRVAL_P(return_value));
			Z_STRVAL_P(return_value) = estrndup(Z_STRVAL_P(return_value),Z_STRLEN_P(return_value));
			Z_TYPE_P(return_value) = IS_STRING;
			break;
		case PHP_PG_FIELD_SIZE:
			Z_LVAL_P(return_value) = PQfsize(pgsql_result, field);
			Z_TYPE_P(return_value) = IS_LONG;
			break;
		case PHP_PG_FIELD_TYPE:
			Z_STRVAL_P(return_value) = get_field_name(pg_result->conn, PQftype(pgsql_result, field), &EG(regular_list) TSRMLS_CC);
			Z_STRLEN_P(return_value) = strlen(Z_STRVAL_P(return_value));
			Z_TYPE_P(return_value) = IS_STRING;
			break;
		case PHP_PG_FIELD_TYPE_OID:
			
			oid = PQftype(pgsql_result, field);
#if UINT_MAX > LONG_MAX
			if (oid > LONG_MAX) {
				smart_str s = {0};
				smart_str_append_unsigned(&s, oid);
				smart_str_0(&s);
				Z_STRVAL_P(return_value) = s.c;
				Z_STRLEN_P(return_value) = s.len;
				Z_TYPE_P(return_value) = IS_STRING;
			} else
#endif
			{
				Z_LVAL_P(return_value) = (long)oid;
				Z_TYPE_P(return_value) = IS_LONG;
			}
			break;
		default:
			RETURN_FALSE;
	}
}

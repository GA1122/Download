PHP_PGSQL_API int php_pgsql_result2array(PGresult *pg_result, zval *ret_array TSRMLS_DC) 
{
	zval *row;
	char *field_name;
	size_t num_fields;
	int pg_numrows, pg_row;
	uint i;
	assert(Z_TYPE_P(ret_array) == IS_ARRAY);

	if ((pg_numrows = PQntuples(pg_result)) <= 0) {
		return FAILURE;
	}
	for (pg_row = 0; pg_row < pg_numrows; pg_row++) {
		MAKE_STD_ZVAL(row);
		array_init(row);
		add_index_zval(ret_array, pg_row, row);
		for (i = 0, num_fields = PQnfields(pg_result); i < num_fields; i++) {
			if (PQgetisnull(pg_result, pg_row, i)) {
				field_name = PQfname(pg_result, i);
				add_assoc_null(row, field_name);
			} else {
				char *element = PQgetvalue(pg_result, pg_row, i);
				if (element) {
					char *data;
					size_t data_len;
					const size_t element_len = strlen(element);

					data = safe_estrndup(element, element_len);
					data_len = element_len;
					
					field_name = PQfname(pg_result, i);
					add_assoc_stringl(row, field_name, data, data_len, 0);
				}
			}
		}
	}
	return SUCCESS;
}

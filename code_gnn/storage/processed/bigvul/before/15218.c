PHP_FUNCTION(pg_meta_data)
{
	zval *pgsql_link;
	char *table_name;
	uint table_name_len;
	PGconn *pgsql;
	int id = -1;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs",
							  &pgsql_link, &table_name, &table_name_len) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, &pgsql_link, id, "PostgreSQL link", le_link, le_plink);
	
	array_init(return_value);
	if (php_pgsql_meta_data(pgsql, table_name, return_value TSRMLS_CC) == FAILURE) {
		zval_dtor(return_value);  
		RETURN_FALSE;
	}
	else {
		HashPosition pos;
		zval **val;

		for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(return_value), &pos);
			zend_hash_get_current_data_ex(Z_ARRVAL_P(return_value), (void **)&val, &pos) == SUCCESS;
			zend_hash_move_forward_ex(Z_ARRVAL_P(return_value), &pos)) {
			 
			zend_hash_del_key_or_index(Z_ARRVAL_PP(val), "is enum", sizeof("is enum"), 0, HASH_DEL_KEY);
		}
	}
}

static PHP_METHOD(PDOStatement, debugDumpParams)
{
	php_stream *out = php_stream_open_wrapper("php://output", "w", 0, NULL);
	HashPosition pos;
	struct pdo_bound_param_data *param;
	PHP_STMT_GET_OBJ;

	if (out == NULL) {
		RETURN_FALSE;
	}

	php_stream_printf(out TSRMLS_CC, "SQL: [%d] %.*s\n",
		stmt->query_stringlen,
		stmt->query_stringlen, stmt->query_string);

	php_stream_printf(out TSRMLS_CC, "Params:  %d\n",
		stmt->bound_params ? zend_hash_num_elements(stmt->bound_params) : 0);

	if (stmt->bound_params) {
		zend_hash_internal_pointer_reset_ex(stmt->bound_params, &pos);
		while (SUCCESS == zend_hash_get_current_data_ex(stmt->bound_params,
				(void**)&param, &pos)) {
			char *str;
			uint len;
			ulong num;
			int res;

			res = zend_hash_get_current_key_ex(stmt->bound_params, &str, &len, &num, 0, &pos);
			if (res == HASH_KEY_IS_LONG) {
				php_stream_printf(out TSRMLS_CC, "Key: Position #%ld:\n", num);
			} else if (res == HASH_KEY_IS_STRING) {
				php_stream_printf(out TSRMLS_CC, "Key: Name: [%d] %.*s\n", len, len, str);
			}

			php_stream_printf(out TSRMLS_CC, "paramno=%ld\nname=[%d] \"%.*s\"\nis_param=%d\nparam_type=%d\n",
				param->paramno, param->namelen, param->namelen, param->name ? param->name : "",
				param->is_param,
				param->param_type);

			zend_hash_move_forward_ex(stmt->bound_params, &pos);
		}
	}

	php_stream_close(out);
}

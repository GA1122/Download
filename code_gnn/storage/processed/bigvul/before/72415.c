static int do_fetch_common(pdo_stmt_t *stmt, enum pdo_fetch_orientation ori,
	long offset, int do_bind TSRMLS_DC)  
{
	if (!stmt->executed) {
		return 0;
	}

	if (!dispatch_param_event(stmt, PDO_PARAM_EVT_FETCH_PRE TSRMLS_CC)) {
		return 0;
	}

	if (!stmt->methods->fetcher(stmt, ori, offset TSRMLS_CC)) {
		return 0;
	}

	 
	if (!stmt->columns && !pdo_stmt_describe_columns(stmt TSRMLS_CC)) {
		return 0;
	}

	if (!dispatch_param_event(stmt, PDO_PARAM_EVT_FETCH_POST TSRMLS_CC)) {
		return 0;
	}

	if (do_bind && stmt->bound_columns) {
		 
		struct pdo_bound_param_data *param;

		zend_hash_internal_pointer_reset(stmt->bound_columns);
		while (SUCCESS == zend_hash_get_current_data(stmt->bound_columns, (void**)&param)) {
			if (param->paramno >= 0) {
				convert_to_string(param->parameter);

				 
				zval_dtor(param->parameter);

				 
				fetch_value(stmt, param->parameter, param->paramno, (int *)&param->param_type TSRMLS_CC);

				 
			}

			zend_hash_move_forward(stmt->bound_columns);
		}
	}

	return 1;
}
 

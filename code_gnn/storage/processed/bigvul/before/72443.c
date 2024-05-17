static inline int rewrite_name_to_position(pdo_stmt_t *stmt, struct pdo_bound_param_data *param TSRMLS_DC)  
{
	if (stmt->bound_param_map) {
		 
		char *name;
		int position = 0;

		if (stmt->named_rewrite_template) {
			 
			return 1;
		}
		if (!param->name) {
			 
			if (SUCCESS == zend_hash_index_find(stmt->bound_param_map, param->paramno, (void**)&name)) {
				param->name = estrdup(name);
				param->namelen = strlen(param->name);
				return 1;
			}
			pdo_raise_impl_error(stmt->dbh, stmt, "HY093", "parameter was not defined" TSRMLS_CC);
			return 0;
		}

		zend_hash_internal_pointer_reset(stmt->bound_param_map);
		while (SUCCESS == zend_hash_get_current_data(stmt->bound_param_map, (void**)&name)) {
			if (strcmp(name, param->name)) {
				position++;
				zend_hash_move_forward(stmt->bound_param_map);
				continue;
			}
			if (param->paramno >= 0) {
				pdo_raise_impl_error(stmt->dbh, stmt, "IM001", "PDO refuses to handle repeating the same :named parameter for multiple positions with this driver, as it might be unsafe to do so.  Consider using a separate name for each parameter instead" TSRMLS_CC);
				return -1;
			}
			param->paramno = position;
			return 1;
		}
		pdo_raise_impl_error(stmt->dbh, stmt, "HY093", "parameter was not defined" TSRMLS_CC);
		return 0;
	}
	return 1;
}
 

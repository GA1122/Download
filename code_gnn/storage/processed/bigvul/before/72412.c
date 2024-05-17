static int dispatch_param_event(pdo_stmt_t *stmt, enum pdo_param_event event_type TSRMLS_DC)  
{
	int ret = 1, is_param = 1;
	struct pdo_bound_param_data *param;
	HashTable *ht;

	if (!stmt->methods->param_hook) {
		return 1;
	}

	ht = stmt->bound_params;

iterate:
	if (ht) {
		zend_hash_internal_pointer_reset(ht);
		while (SUCCESS == zend_hash_get_current_data(ht, (void**)&param)) {
			if (!stmt->methods->param_hook(stmt, param, event_type TSRMLS_CC)) {
				ret = 0;
				break;
			}

			zend_hash_move_forward(ht);
		}
	}
	if (ret && is_param) {
		ht = stmt->bound_columns;
		is_param = 0;
		goto iterate;
	}

	return ret;
}
 
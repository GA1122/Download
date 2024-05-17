static int really_register_bound_param(struct pdo_bound_param_data *param, pdo_stmt_t *stmt, int is_param TSRMLS_DC)  
{
	HashTable *hash;
	struct pdo_bound_param_data *pparam = NULL;

	hash = is_param ? stmt->bound_params : stmt->bound_columns;

	if (!hash) {
		ALLOC_HASHTABLE(hash);
		zend_hash_init(hash, 13, NULL, param_dtor, 0);

		if (is_param) {
			stmt->bound_params = hash;
		} else {
			stmt->bound_columns = hash;
		}
	}

	if (PDO_PARAM_TYPE(param->param_type) == PDO_PARAM_STR && param->max_value_len <= 0 && ! ZVAL_IS_NULL(param->parameter)) {
		if (Z_TYPE_P(param->parameter) == IS_DOUBLE) {
			char *p;
			int len = spprintf(&p, 0, "%.*H", (int) EG(precision), Z_DVAL_P(param->parameter));
			ZVAL_STRINGL(param->parameter, p, len, 0);
		} else {
			convert_to_string(param->parameter);
		}
	} else if (PDO_PARAM_TYPE(param->param_type) == PDO_PARAM_INT && Z_TYPE_P(param->parameter) == IS_BOOL) {
		convert_to_long(param->parameter);
	} else if (PDO_PARAM_TYPE(param->param_type) == PDO_PARAM_BOOL && Z_TYPE_P(param->parameter) == IS_LONG) {
		convert_to_boolean(param->parameter);
	}

	param->stmt = stmt;
	param->is_param = is_param;

	if (param->driver_params) {
		Z_ADDREF_P(param->driver_params);
	}

	if (!is_param && param->name && stmt->columns) {
		 
		int i;

		for (i = 0; i < stmt->column_count; i++) {
			if (strcmp(stmt->columns[i].name, param->name) == 0) {
				param->paramno = i;
				break;
			}
		}

		 
		if (param->paramno == -1) {
			char *tmp;
			spprintf(&tmp, 0, "Did not find column name '%s' in the defined columns; it will not be bound", param->name);
			pdo_raise_impl_error(stmt->dbh, stmt, "HY000", tmp TSRMLS_CC);
			efree(tmp);
		}
	}

	if (param->name) {
		if (is_param && param->name[0] != ':') {
			char *temp = emalloc(++param->namelen + 1);
			temp[0] = ':';
			memmove(temp+1, param->name, param->namelen);
			param->name = temp;
		} else {
			param->name = estrndup(param->name, param->namelen);
		}
	}

	if (is_param && !rewrite_name_to_position(stmt, param TSRMLS_CC)) {
		if (param->name) {
			efree(param->name);
			param->name = NULL;
		}
		return 0;
	}

	 
	if (stmt->methods->param_hook) {
		if (!stmt->methods->param_hook(stmt, param, PDO_PARAM_EVT_NORMALIZE
				TSRMLS_CC)) {
			if (param->name) {
				efree(param->name);
				param->name = NULL;
			}
			return 0;
		}
	}

	 
	if (param->paramno >= 0) {
		zend_hash_index_del(hash, param->paramno);
	}

	 
	if (param->name) {
		zend_hash_update(hash, param->name, param->namelen, param,
			sizeof(*param), (void**)&pparam);
	} else {
		zend_hash_index_update(hash, param->paramno, param, sizeof(*param),
			(void**)&pparam);
	}

	 
	if (stmt->methods->param_hook) {
		if (!stmt->methods->param_hook(stmt, pparam, PDO_PARAM_EVT_ALLOC
					TSRMLS_CC)) {
			 
			if (pparam->name) {
				zend_hash_del(hash, pparam->name, pparam->namelen);
			} else {
				zend_hash_index_del(hash, pparam->paramno);
			}
			 
			param->parameter = NULL;
			return 0;
		}
	}
	return 1;
}
 

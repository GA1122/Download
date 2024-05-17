static inline void fetch_value(pdo_stmt_t *stmt, zval *dest, int colno, int *type_override TSRMLS_DC)  
{
	struct pdo_column_data *col;
	char *value = NULL;
	unsigned long value_len = 0;
	int caller_frees = 0;
	int type, new_type;

	col = &stmt->columns[colno];
	type = PDO_PARAM_TYPE(col->param_type);
	new_type =  type_override ? PDO_PARAM_TYPE(*type_override) : type;

	value = NULL;
	value_len = 0;

	stmt->methods->get_col(stmt, colno, &value, &value_len, &caller_frees TSRMLS_CC);

	switch (type) {
		case PDO_PARAM_ZVAL:
			if (value && value_len == sizeof(zval)) {
				int need_copy = (new_type != PDO_PARAM_ZVAL || stmt->dbh->stringify) ? 1 : 0;
				zval *zv = *(zval**)value;
				ZVAL_ZVAL(dest, zv, need_copy, 1);
			} else {
				ZVAL_NULL(dest);
			}

			if (Z_TYPE_P(dest) == IS_NULL) {
				type = new_type;
			}
			break;

		case PDO_PARAM_INT:
			if (value && value_len == sizeof(long)) {
				ZVAL_LONG(dest, *(long*)value);
				break;
			}
			ZVAL_NULL(dest);
			break;

		case PDO_PARAM_BOOL:
			if (value && value_len == sizeof(zend_bool)) {
				ZVAL_BOOL(dest, *(zend_bool*)value);
				break;
			}
			ZVAL_NULL(dest);
			break;

		case PDO_PARAM_LOB:
			if (value == NULL) {
				ZVAL_NULL(dest);
			} else if (value_len == 0) {
				 
				if (stmt->dbh->stringify || new_type == PDO_PARAM_STR) {
					char *buf = NULL;
					size_t len;
					len = php_stream_copy_to_mem((php_stream*)value, &buf, PHP_STREAM_COPY_ALL, 0);
					if(buf == NULL) {
						ZVAL_EMPTY_STRING(dest);
					} else {
						ZVAL_STRINGL(dest, buf, len, 0);
					}
					php_stream_close((php_stream*)value);
				} else {
					php_stream_to_zval((php_stream*)value, dest);
				}
			} else if (!stmt->dbh->stringify && new_type != PDO_PARAM_STR) {
				 
				php_stream *stm;
#ifdef TEMP_STREAM_TAKE_BUFFER
				if (caller_frees) {
					stm = php_stream_memory_open(TEMP_STREAM_TAKE_BUFFER, value, value_len);
					if (stm) {
						caller_frees = 0;
					}
				} else
#endif
				{
					stm = php_stream_memory_open(TEMP_STREAM_READONLY, value, value_len);
				}
				if (stm) {
					php_stream_to_zval(stm, dest);
				} else {
					ZVAL_NULL(dest);
				}
			} else {
				ZVAL_STRINGL(dest, value, value_len, !caller_frees);
				if (caller_frees) {
					caller_frees = 0;
				}
			}
			break;

		case PDO_PARAM_STR:
			if (value && !(value_len == 0 && stmt->dbh->oracle_nulls == PDO_NULL_EMPTY_STRING)) {
				ZVAL_STRINGL(dest, value, value_len, !caller_frees);
				if (caller_frees) {
					caller_frees = 0;
				}
				break;
			}
		default:
			ZVAL_NULL(dest);
	}

	if (type != new_type) {
		switch (new_type) {
			case PDO_PARAM_INT:
				convert_to_long_ex(&dest);
				break;
			case PDO_PARAM_BOOL:
				convert_to_boolean_ex(&dest);
				break;
			case PDO_PARAM_STR:
				convert_to_string_ex(&dest);
				break;
			case PDO_PARAM_NULL:
				convert_to_null_ex(&dest);
				break;
			default:
				;
		}
	}

	if (caller_frees && value) {
		efree(value);
	}

	if (stmt->dbh->stringify) {
		switch (Z_TYPE_P(dest)) {
			case IS_LONG:
			case IS_DOUBLE:
				convert_to_string(dest);
				break;
		}
	}

	if (Z_TYPE_P(dest) == IS_NULL && stmt->dbh->oracle_nulls == PDO_NULL_TO_STRING) {
		ZVAL_EMPTY_STRING(dest);
	}
}
 
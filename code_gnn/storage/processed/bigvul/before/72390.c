static PHP_METHOD(PDOStatement, fetchAll)
{
	long how = PDO_FETCH_USE_DEFAULT;
	zval *data, *return_all;
	zval *arg2;
	zend_class_entry *old_ce;
	zval *old_ctor_args, *ctor_args = NULL;
	int error = 0, flags, old_arg_count;
	PHP_STMT_GET_OBJ;

	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|lzz", &how, &arg2, &ctor_args)) {
		RETURN_FALSE;
	}

	if (!pdo_stmt_verify_mode(stmt, how, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	old_ce = stmt->fetch.cls.ce;
	old_ctor_args = stmt->fetch.cls.ctor_args;
	old_arg_count = stmt->fetch.cls.fci.param_count;

	do_fetch_opt_finish(stmt, 0 TSRMLS_CC);

	switch(how & ~PDO_FETCH_FLAGS) {
	case PDO_FETCH_CLASS:
		switch(ZEND_NUM_ARGS()) {
		case 0:
		case 1:
			stmt->fetch.cls.ce = zend_standard_class_def;
			break;
		case 3:
			if (Z_TYPE_P(ctor_args) != IS_NULL && Z_TYPE_P(ctor_args) != IS_ARRAY) {
				pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "ctor_args must be either NULL or an array" TSRMLS_CC);
				error = 1;
				break;
			}
			if (Z_TYPE_P(ctor_args) != IS_ARRAY || !zend_hash_num_elements(Z_ARRVAL_P(ctor_args))) {
				ctor_args = NULL;
			}
			 
		case 2:
			stmt->fetch.cls.ctor_args = ctor_args;  
			if (Z_TYPE_P(arg2) != IS_STRING) {
				pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "Invalid class name (should be a string)" TSRMLS_CC);
				error = 1;
				break;
			} else {
				stmt->fetch.cls.ce = zend_fetch_class(Z_STRVAL_P(arg2), Z_STRLEN_P(arg2), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
				if (!stmt->fetch.cls.ce) {
					pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "could not find user-specified class" TSRMLS_CC);
					error = 1;
					break;
				}
			}
		}
		if (!error) {
			do_fetch_class_prepare(stmt TSRMLS_CC);
		}
		break;

	case PDO_FETCH_FUNC:
		switch(ZEND_NUM_ARGS()) {
		case 0:
		case 1:
			pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "no fetch function specified" TSRMLS_CC);
			error = 1;
			break;
		case 3:
		case 2:
			stmt->fetch.func.function = arg2;
			if (do_fetch_func_prepare(stmt TSRMLS_CC) == 0) {
				error = 1;
			}
			break;
		}
		break;

	case PDO_FETCH_COLUMN:
		switch(ZEND_NUM_ARGS()) {
		case 0:
		case 1:
			stmt->fetch.column = how & PDO_FETCH_GROUP ? -1 : 0;
			break;
		case 2:
			convert_to_long(arg2);
			stmt->fetch.column = Z_LVAL_P(arg2);
			break;
		case 3:
			pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "Third parameter not allowed for PDO::FETCH_COLUMN" TSRMLS_CC);
			error = 1;
		}
		break;

	default:
		if (ZEND_NUM_ARGS() > 1) {
			pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "Extraneous additional parameters" TSRMLS_CC);
			error = 1;
		}
	}

	flags = how & PDO_FETCH_FLAGS;

	if ((how & ~PDO_FETCH_FLAGS) == PDO_FETCH_USE_DEFAULT) {
		flags |= stmt->default_fetch_type & PDO_FETCH_FLAGS;
		how |= stmt->default_fetch_type & ~PDO_FETCH_FLAGS;
	}

	if (!error)	{
		PDO_STMT_CLEAR_ERR();
		MAKE_STD_ZVAL(data);
		if (	(how & PDO_FETCH_GROUP) || how == PDO_FETCH_KEY_PAIR ||
			(how == PDO_FETCH_USE_DEFAULT && stmt->default_fetch_type == PDO_FETCH_KEY_PAIR)
		) {
			array_init(return_value);
			return_all = return_value;
		} else {
			return_all = 0;
		}
		if (!do_fetch(stmt, TRUE, data, how | flags, PDO_FETCH_ORI_NEXT, 0, return_all TSRMLS_CC)) {
			FREE_ZVAL(data);
			error = 2;
		}
	}
	if (!error) {
		if ((how & PDO_FETCH_GROUP)) {
			do {
				MAKE_STD_ZVAL(data);
			} while (do_fetch(stmt, TRUE, data, how | flags, PDO_FETCH_ORI_NEXT, 0, return_all TSRMLS_CC));
		} else if (how == PDO_FETCH_KEY_PAIR || (how == PDO_FETCH_USE_DEFAULT && stmt->default_fetch_type == PDO_FETCH_KEY_PAIR)) {
			while (do_fetch(stmt, TRUE, data, how | flags, PDO_FETCH_ORI_NEXT, 0, return_all TSRMLS_CC));
		} else {
			array_init(return_value);
			do {
				add_next_index_zval(return_value, data);
				MAKE_STD_ZVAL(data);
			} while (do_fetch(stmt, TRUE, data, how | flags, PDO_FETCH_ORI_NEXT, 0, 0 TSRMLS_CC));
		}
		FREE_ZVAL(data);
	}

	do_fetch_opt_finish(stmt, 0 TSRMLS_CC);

	stmt->fetch.cls.ce = old_ce;
	stmt->fetch.cls.ctor_args = old_ctor_args;
	stmt->fetch.cls.fci.param_count = old_arg_count;

	if (error) {
		PDO_HANDLE_STMT_ERR();
		if (error != 2) {
			RETURN_FALSE;
		} else {  
			if (Z_TYPE_P(return_value) != IS_ARRAY) {
				array_init(return_value);
			}
			return;
		}
	}
}

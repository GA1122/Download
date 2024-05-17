static PHP_METHOD(PDOStatement, fetchObject)
{
	long how = PDO_FETCH_CLASS;
	long ori = PDO_FETCH_ORI_NEXT;
	long off = 0;
	char *class_name = NULL;
	int class_name_len;
	zend_class_entry *old_ce;
	zval *old_ctor_args, *ctor_args = NULL;
	int error = 0, old_arg_count;

	PHP_STMT_GET_OBJ;

	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s!a", &class_name, &class_name_len, &ctor_args)) {
		RETURN_FALSE;
	}

	PDO_STMT_CLEAR_ERR();

	if (!pdo_stmt_verify_mode(stmt, how, 0 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	old_ce = stmt->fetch.cls.ce;
	old_ctor_args = stmt->fetch.cls.ctor_args;
	old_arg_count = stmt->fetch.cls.fci.param_count;

	do_fetch_opt_finish(stmt, 0 TSRMLS_CC);

	if (ctor_args) {
		if (Z_TYPE_P(ctor_args) == IS_ARRAY && zend_hash_num_elements(Z_ARRVAL_P(ctor_args))) {
			ALLOC_ZVAL(stmt->fetch.cls.ctor_args);
			*stmt->fetch.cls.ctor_args = *ctor_args;
			zval_copy_ctor(stmt->fetch.cls.ctor_args);
		} else {
			stmt->fetch.cls.ctor_args = NULL;
		}
	}
	if (class_name && !error) {
		stmt->fetch.cls.ce = zend_fetch_class(class_name, class_name_len, ZEND_FETCH_CLASS_AUTO TSRMLS_CC);

		if (!stmt->fetch.cls.ce) {
			pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "Could not find user-supplied class" TSRMLS_CC);
			error = 1;
		}
	} else if (!error) {
		stmt->fetch.cls.ce = zend_standard_class_def;
	}

	if (!error && !do_fetch(stmt, TRUE, return_value, how, ori, off, 0 TSRMLS_CC)) {
		error = 1;
	}
	if (error) {
		PDO_HANDLE_STMT_ERR();
	}
	do_fetch_opt_finish(stmt, 1 TSRMLS_CC);

	stmt->fetch.cls.ce = old_ce;
	stmt->fetch.cls.ctor_args = old_ctor_args;
	stmt->fetch.cls.fci.param_count = old_arg_count;
	if (error) {
		RETURN_FALSE;
	}
}

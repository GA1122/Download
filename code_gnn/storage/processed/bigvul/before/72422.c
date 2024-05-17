static int make_callable_ex(pdo_stmt_t *stmt, zval *callable, zend_fcall_info * fci, zend_fcall_info_cache * fcc, int num_args TSRMLS_DC)  
{
	char *is_callable_error = NULL;

	if (zend_fcall_info_init(callable, 0, fci, fcc, NULL, &is_callable_error TSRMLS_CC) == FAILURE) {
		if (is_callable_error) {
			pdo_raise_impl_error(stmt->dbh, stmt, "HY000", is_callable_error TSRMLS_CC);
			efree(is_callable_error);
		} else {
			pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "user-supplied function must be a valid callback" TSRMLS_CC);
		}
		return 0;
	}
	if (is_callable_error) {
		 
		efree(is_callable_error);
	}

	fci->param_count = num_args;  
	fci->params = safe_emalloc(sizeof(zval**), num_args, 0);

	return 1;
}
 
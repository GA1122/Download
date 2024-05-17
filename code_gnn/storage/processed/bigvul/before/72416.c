static int do_fetch_func_prepare(pdo_stmt_t *stmt TSRMLS_DC)  
{
	zend_fcall_info * fci = &stmt->fetch.cls.fci;
	zend_fcall_info_cache * fcc = &stmt->fetch.cls.fcc;

	if (!make_callable_ex(stmt, stmt->fetch.func.function, fci, fcc, stmt->column_count TSRMLS_CC)) {
		return 0;
	} else {
		stmt->fetch.func.values = safe_emalloc(sizeof(zval*), stmt->column_count, 0);
		return 1;
	}
}
 
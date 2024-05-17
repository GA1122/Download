static int do_fetch_opt_finish(pdo_stmt_t *stmt, int free_ctor_agrs TSRMLS_DC)  
{
	 
	if (stmt->fetch.cls.fci.size && stmt->fetch.cls.fci.params) {
		efree(stmt->fetch.cls.fci.params);
		stmt->fetch.cls.fci.params = NULL;
	}
	stmt->fetch.cls.fci.size = 0;
	if (stmt->fetch.cls.ctor_args && free_ctor_agrs) {
		zval_ptr_dtor(&stmt->fetch.cls.ctor_args);
		stmt->fetch.cls.ctor_args = NULL;
		stmt->fetch.cls.fci.param_count = 0;
	}
	if (stmt->fetch.func.values) {
		efree(stmt->fetch.func.values);
		stmt->fetch.func.values = NULL;
	}
	return 1;
}
 
static int do_fetch_class_prepare(pdo_stmt_t *stmt TSRMLS_DC)  
{
	zend_class_entry * ce = stmt->fetch.cls.ce;
	zend_fcall_info * fci = &stmt->fetch.cls.fci;
	zend_fcall_info_cache * fcc = &stmt->fetch.cls.fcc;

	fci->size = sizeof(zend_fcall_info);

	if (!ce) {
		stmt->fetch.cls.ce = ZEND_STANDARD_CLASS_DEF_PTR;
		ce = ZEND_STANDARD_CLASS_DEF_PTR;
	}

	if (ce->constructor) {
		fci->function_table = &ce->function_table;
		fci->function_name = NULL;
		fci->symbol_table = NULL;
		fci->retval_ptr_ptr = &stmt->fetch.cls.retval_ptr;
		fci->params = NULL;
		fci->no_separation = 1;

		zend_fcall_info_args(fci, stmt->fetch.cls.ctor_args TSRMLS_CC);

		fcc->initialized = 1;
		fcc->function_handler = ce->constructor;
		fcc->calling_scope = EG(scope);
		fcc->called_scope = ce;
		return 1;
	} else if (stmt->fetch.cls.ctor_args) {
		pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "user-supplied class does not have a constructor, use NULL for the ctor_params parameter, or simply omit it" TSRMLS_CC);
		return 0;
	} else {
		return 1;  
	}
}
 
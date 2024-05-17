static void destroy_phar_data(void *pDest)  
{
	phar_archive_data *phar_data = *(phar_archive_data **) pDest;
	TSRMLS_FETCH();

	if (PHAR_GLOBALS->request_ends) {
		 
		zend_hash_apply(&(phar_data->manifest), phar_tmpclose_apply TSRMLS_CC);
		destroy_phar_data_only(pDest);
		return;
	}

	zend_hash_apply_with_argument(&(PHAR_GLOBALS->phar_alias_map), phar_unalias_apply, phar_data TSRMLS_CC);

	if (--phar_data->refcount < 0) {
		phar_destroy_phar_data(phar_data TSRMLS_CC);
	}
}
 

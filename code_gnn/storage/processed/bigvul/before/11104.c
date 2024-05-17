static void destroy_phar_data(zval *zv)  
{
	phar_archive_data *phar_data = (phar_archive_data *)Z_PTR_P(zv);

	if (PHAR_G(request_ends)) {
		 
		zend_hash_apply(&(phar_data->manifest), phar_tmpclose_apply);
		destroy_phar_data_only(zv);
		return;
	}

	zend_hash_apply_with_argument(&(PHAR_G(phar_alias_map)), phar_unalias_apply, phar_data);

	if (--phar_data->refcount < 0) {
		phar_destroy_phar_data(phar_data);
	}
}
 

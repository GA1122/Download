int phar_free_alias(phar_archive_data *phar, char *alias, int alias_len TSRMLS_DC)  
{
	if (phar->refcount || phar->is_persistent) {
		return FAILURE;
	}

	 
	if (zend_hash_del(&(PHAR_GLOBALS->phar_fname_map), phar->fname, phar->fname_len) != SUCCESS) {
		return FAILURE;
	}

	 
	PHAR_G(last_phar) = NULL;
	PHAR_G(last_phar_name) = PHAR_G(last_alias) = NULL;

	return SUCCESS;
}
 

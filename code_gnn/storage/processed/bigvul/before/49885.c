int phar_free_alias(phar_archive_data *phar, char *alias, int alias_len)  
{
	if (phar->refcount || phar->is_persistent) {
		return FAILURE;
	}

	 
	if (zend_hash_str_del(&(PHAR_G(phar_fname_map)), phar->fname, phar->fname_len) != SUCCESS) {
		return FAILURE;
	}

	 
	PHAR_G(last_phar) = NULL;
	PHAR_G(last_phar_name) = PHAR_G(last_alias) = NULL;

	return SUCCESS;
}
 
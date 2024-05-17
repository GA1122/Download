int phar_archive_delref(phar_archive_data *phar)  
{
	if (phar->is_persistent) {
		return 0;
	}

	if (--phar->refcount < 0) {
		if (PHAR_G(request_done)
		|| zend_hash_str_del(&(PHAR_G(phar_fname_map)), phar->fname, phar->fname_len) != SUCCESS) {
			phar_destroy_phar_data(phar);
		}
		return 1;
	} else if (!phar->refcount) {
		 
		PHAR_G(last_phar) = NULL;
		PHAR_G(last_phar_name) = PHAR_G(last_alias) = NULL;

		if (phar->fp && !(phar->flags & PHAR_FILE_COMPRESSION_MASK)) {
			 
			php_stream_close(phar->fp);
			phar->fp = NULL;
		}

		if (!zend_hash_num_elements(&phar->manifest)) {
			 
			if (zend_hash_str_del(&(PHAR_G(phar_fname_map)), phar->fname, phar->fname_len) != SUCCESS) {
				phar_destroy_phar_data(phar);
			}
			return 1;
		}
	}
	return 0;
}
 

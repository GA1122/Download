void phar_destroy_phar_data(phar_archive_data *phar TSRMLS_DC)  
{
	if (phar->alias && phar->alias != phar->fname) {
		pefree(phar->alias, phar->is_persistent);
		phar->alias = NULL;
	}

	if (phar->fname) {
		pefree(phar->fname, phar->is_persistent);
		phar->fname = NULL;
	}

	if (phar->signature) {
		pefree(phar->signature, phar->is_persistent);
		phar->signature = NULL;
	}

	if (phar->manifest.arBuckets) {
		zend_hash_destroy(&phar->manifest);
		phar->manifest.arBuckets = NULL;
	}

	if (phar->mounted_dirs.arBuckets) {
		zend_hash_destroy(&phar->mounted_dirs);
		phar->mounted_dirs.arBuckets = NULL;
	}

	if (phar->virtual_dirs.arBuckets) {
		zend_hash_destroy(&phar->virtual_dirs);
		phar->virtual_dirs.arBuckets = NULL;
	}

	if (phar->metadata) {
		if (phar->is_persistent) {
			if (phar->metadata_len) {
				 
				free(phar->metadata);
			} else {
				zval_internal_ptr_dtor(&phar->metadata);
			}
		} else {
			zval_ptr_dtor(&phar->metadata);
		}
		phar->metadata_len = 0;
		phar->metadata = 0;
	}

	if (phar->fp) {
		php_stream_close(phar->fp);
		phar->fp = 0;
	}

	if (phar->ufp) {
		php_stream_close(phar->ufp);
		phar->ufp = 0;
	}

	pefree(phar, phar->is_persistent);
}
 
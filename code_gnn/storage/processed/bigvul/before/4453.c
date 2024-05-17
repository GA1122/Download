void destroy_phar_manifest_entry(void *pDest)  
{
	phar_entry_info *entry = (phar_entry_info *)pDest;
	TSRMLS_FETCH();

	if (entry->cfp) {
		php_stream_close(entry->cfp);
		entry->cfp = 0;
	}

	if (entry->fp) {
		php_stream_close(entry->fp);
		entry->fp = 0;
	}

	if (entry->metadata) {
		if (entry->is_persistent) {
			if (entry->metadata_len) {
				 
				free(entry->metadata);
			} else {
				zval_internal_ptr_dtor(&entry->metadata);
			}
		} else {
			zval_ptr_dtor(&entry->metadata);
		}
		entry->metadata_len = 0;
		entry->metadata = 0;
	}

	if (entry->metadata_str.c) {
		smart_str_free(&entry->metadata_str);
		entry->metadata_str.c = 0;
	}

	pefree(entry->filename, entry->is_persistent);

	if (entry->link) {
		pefree(entry->link, entry->is_persistent);
		entry->link = 0;
	}

	if (entry->tmp) {
		pefree(entry->tmp, entry->is_persistent);
		entry->tmp = 0;
	}
}
 
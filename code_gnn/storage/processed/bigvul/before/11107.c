void destroy_phar_manifest_entry_int(phar_entry_info *entry)  
{

	if (entry->cfp) {
		php_stream_close(entry->cfp);
		entry->cfp = 0;
	}

	if (entry->fp) {
		php_stream_close(entry->fp);
		entry->fp = 0;
	}

	if (Z_TYPE(entry->metadata) != IS_UNDEF) {
		if (entry->is_persistent) {
			if (entry->metadata_len) {
				 
				free(Z_PTR(entry->metadata));
			} else {
				zval_internal_ptr_dtor(&entry->metadata);
			}
		} else {
			zval_ptr_dtor(&entry->metadata);
		}
		entry->metadata_len = 0;
		ZVAL_UNDEF(&entry->metadata);
	}

	if (entry->metadata_str.s) {
		smart_str_free(&entry->metadata_str);
		entry->metadata_str.s = NULL;
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
 

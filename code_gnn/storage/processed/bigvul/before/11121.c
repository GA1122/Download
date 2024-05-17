static int phar_flush_clean_deleted_apply(zval *zv)  
{
	phar_entry_info *entry = (phar_entry_info *)Z_PTR_P(zv);

	if (entry->fp_refcount <= 0 && entry->is_deleted) {
		return ZEND_HASH_APPLY_REMOVE;
	} else {
		return ZEND_HASH_APPLY_KEEP;
	}
}
 

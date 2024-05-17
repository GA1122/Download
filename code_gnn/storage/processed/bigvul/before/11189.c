static int phar_set_compression(zval *zv, void *argument)  
{
	phar_entry_info *entry = (phar_entry_info *)Z_PTR_P(zv);
	php_uint32 compress = *(php_uint32 *)argument;

	if (entry->is_deleted) {
		return ZEND_HASH_APPLY_KEEP;
	}

	entry->old_flags = entry->flags;
	entry->flags &= ~PHAR_ENT_COMPRESSION_MASK;
	entry->flags |= compress;
	entry->is_modified = 1;
	return ZEND_HASH_APPLY_KEEP;
}
 
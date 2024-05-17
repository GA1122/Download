ZEND_API zval *_zend_ts_hash_str_update(TsHashTable *ht, const char *key, size_t len, zval *pData ZEND_FILE_LINE_DC)
{
	zval *retval;

	begin_write(ht);
	retval = zend_hash_str_update(TS_HASH(ht), key, len, pData);
	end_write(ht);

	return retval;
}

ZEND_API zval *_zend_ts_hash_add_or_update(TsHashTable *ht, zend_string *key, zval *pData, int flag ZEND_FILE_LINE_DC)
{
	zval *retval;

	begin_write(ht);
	retval = _zend_hash_add_or_update(TS_HASH(ht), key, pData, flag ZEND_FILE_LINE_RELAY_CC);
	end_write(ht);

	return retval;
}

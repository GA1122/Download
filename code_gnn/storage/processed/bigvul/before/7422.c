ZEND_API zval *zend_ts_hash_find(TsHashTable *ht, zend_string *key)
{
	zval *retval;

	begin_read(ht);
	retval = zend_hash_find(TS_HASH(ht), key);
	end_read(ht);

	return retval;
}

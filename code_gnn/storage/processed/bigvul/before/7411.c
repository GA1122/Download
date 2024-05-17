ZEND_API zval *zend_ts_hash_add_empty_element(TsHashTable *ht, zend_string *key)
{
	zval *retval;

	begin_write(ht);
	retval = zend_hash_add_empty_element(TS_HASH(ht), key);
	end_write(ht);

	return retval;
}

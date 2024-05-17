ZEND_API zval *zend_ts_hash_index_find(TsHashTable *ht, zend_ulong h)
{
	zval *retval;

	begin_read(ht);
	retval = zend_hash_index_find(TS_HASH(ht), h);
	end_read(ht);

	return retval;
}

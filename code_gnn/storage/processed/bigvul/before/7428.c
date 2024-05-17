ZEND_API zval *zend_ts_hash_minmax(TsHashTable *ht, compare_func_t compar, int flag)
{
	zval *retval;

	begin_read(ht);
	retval = zend_hash_minmax(TS_HASH(ht), compar, flag);
	end_read(ht);

	return retval;
}

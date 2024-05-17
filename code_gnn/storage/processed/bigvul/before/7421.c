ZEND_API int zend_ts_hash_exists(TsHashTable *ht, zend_string *key)
{
	int retval;

	begin_read(ht);
	retval = zend_hash_exists(TS_HASH(ht), key);
	end_read(ht);

	return retval;
}

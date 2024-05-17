ZEND_API int zend_ts_hash_del(TsHashTable *ht, zend_string *key)
{
	int retval;

	begin_write(ht);
	retval = zend_hash_del(TS_HASH(ht), key);
	end_write(ht);

	return retval;
}

ZEND_API int zend_ts_hash_index_exists(TsHashTable *ht, zend_ulong h)
{
	int retval;

	begin_read(ht);
	retval = zend_hash_index_exists(TS_HASH(ht), h);
	end_read(ht);

	return retval;
}

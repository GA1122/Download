ZEND_API int zend_ts_hash_index_del(TsHashTable *ht, zend_ulong h)
{
	int retval;

	begin_write(ht);
	retval = zend_hash_index_del(TS_HASH(ht), h);
	end_write(ht);

	return retval;
}

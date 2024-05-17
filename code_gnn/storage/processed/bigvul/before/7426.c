ZEND_API void zend_ts_hash_merge(TsHashTable *target, TsHashTable *source, copy_ctor_func_t pCopyConstructor, int overwrite)
{
	begin_read(source);
	begin_write(target);
	zend_hash_merge(TS_HASH(target), TS_HASH(source), pCopyConstructor, overwrite);
	end_write(target);
	end_read(source);
}

ZEND_API void zend_ts_hash_reverse_apply(TsHashTable *ht, apply_func_t apply_func)
{
	begin_write(ht);
	zend_hash_reverse_apply(TS_HASH(ht), apply_func);
	end_write(ht);
}

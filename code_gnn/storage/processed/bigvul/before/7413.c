ZEND_API void zend_ts_hash_apply_with_argument(TsHashTable *ht, apply_func_arg_t apply_func, void *argument)
{
	begin_write(ht);
	zend_hash_apply_with_argument(TS_HASH(ht), apply_func, argument);
	end_write(ht);
}

ZEND_API int add_get_index_string(zval *arg, ulong index, const char *str, void **dest, int duplicate)  
{
	zval *tmp;

	MAKE_STD_ZVAL(tmp);
	ZVAL_STRING(tmp, str, duplicate);

	return zend_hash_index_update(Z_ARRVAL_P(arg), index, (void *) &tmp, sizeof(zval *), dest);
}
 

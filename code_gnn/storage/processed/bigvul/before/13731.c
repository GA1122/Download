ZEND_API int add_get_index_stringl(zval *arg, ulong index, const char *str, uint length, void **dest, int duplicate)  
{
	zval *tmp;

	MAKE_STD_ZVAL(tmp);
	ZVAL_STRINGL(tmp, str, length, duplicate);

	return zend_hash_index_update(Z_ARRVAL_P(arg), index, (void *) &tmp, sizeof(zval *), dest);
}
 

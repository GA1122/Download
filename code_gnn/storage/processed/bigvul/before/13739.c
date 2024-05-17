ZEND_API int add_index_zval(zval *arg, ulong index, zval *value)  
{
	return zend_hash_index_update(Z_ARRVAL_P(arg), index, (void *) &value, sizeof(zval *), NULL);
}
 

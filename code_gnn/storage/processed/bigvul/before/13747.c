ZEND_API int add_next_index_zval(zval *arg, zval *value)  
{
	return zend_hash_next_index_insert(Z_ARRVAL_P(arg), &value, sizeof(zval *), NULL);
}
 

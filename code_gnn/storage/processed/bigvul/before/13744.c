ZEND_API int add_next_index_resource(zval *arg, int r)  
{
	zval *tmp;

	MAKE_STD_ZVAL(tmp);
	ZVAL_RESOURCE(tmp, r);

	return zend_hash_next_index_insert(Z_ARRVAL_P(arg), &tmp, sizeof(zval *), NULL);
}
 

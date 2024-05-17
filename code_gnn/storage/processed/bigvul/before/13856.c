ZEND_API char *zend_zval_type_name(const zval *arg)  
{
	return zend_get_type_by_const(Z_TYPE_P(arg));
}
 

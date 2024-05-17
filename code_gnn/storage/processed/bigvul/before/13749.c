ZEND_API int add_property_double_ex(zval *arg, const char *key, uint key_len, double d TSRMLS_DC)  
{
	zval *tmp;
	zval *z_key;

	MAKE_STD_ZVAL(tmp);
	ZVAL_DOUBLE(tmp, d);

	MAKE_STD_ZVAL(z_key);
	ZVAL_STRINGL(z_key, key, key_len-1, 1);

	Z_OBJ_HANDLER_P(arg, write_property)(arg, z_key, tmp, 0 TSRMLS_CC);
	zval_ptr_dtor(&tmp);  
	zval_ptr_dtor(&z_key);
	return SUCCESS;
}
 

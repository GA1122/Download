ZEND_API int add_property_zval_ex(zval *arg, const char *key, uint key_len, zval *value TSRMLS_DC)  
{
	zval *z_key;

	MAKE_STD_ZVAL(z_key);
	ZVAL_STRINGL(z_key, key, key_len-1, 1);

	Z_OBJ_HANDLER_P(arg, write_property)(arg, z_key, value, 0 TSRMLS_CC);
	zval_ptr_dtor(&z_key);
	return SUCCESS;
}
 

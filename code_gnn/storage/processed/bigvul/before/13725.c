ZEND_API int add_assoc_zval_ex(zval *arg, const char *key, uint key_len, zval *value)  
{
	return zend_symtable_update(Z_ARRVAL_P(arg), key, key_len, (void *) &value, sizeof(zval *), NULL);
}
 

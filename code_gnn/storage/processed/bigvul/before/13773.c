ZEND_API int zend_declare_class_constant(zend_class_entry *ce, const char *name, size_t name_length, zval *value TSRMLS_DC)  
{
	return zend_hash_update(&ce->constants_table, name, name_length+1, &value, sizeof(zval *), NULL);
}
 

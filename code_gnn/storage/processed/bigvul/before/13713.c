ZEND_API int _object_init(zval *arg ZEND_FILE_LINE_DC TSRMLS_DC)  
{
	return _object_init_ex(arg, zend_standard_class_def ZEND_FILE_LINE_RELAY_CC TSRMLS_CC);
}
 

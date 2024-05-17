ZEND_API void zend_cleanup_internal_classes(TSRMLS_D)  
{
	zend_class_entry **p = class_cleanup_handlers;

	while (*p) {
		zend_cleanup_internal_class_data(*p TSRMLS_CC);
		p++;
	}
}
 

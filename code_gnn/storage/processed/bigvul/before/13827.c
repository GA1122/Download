ZEND_API zend_class_entry *zend_register_internal_class(zend_class_entry *orig_class_entry TSRMLS_DC)  
{
	return do_register_internal_class(orig_class_entry, 0 TSRMLS_CC);
}
 

static zend_object_value display_disabled_class(zend_class_entry *class_type TSRMLS_DC)  
{
	zend_object_value retval;
	zend_object *intern;
	retval = zend_objects_new(&intern, class_type TSRMLS_CC);
	zend_error(E_WARNING, "%s() has been disabled for security reasons", class_type->name);
	return retval;
}
#ifdef ZEND_WIN32
#pragma optimize("", on)
#endif
 

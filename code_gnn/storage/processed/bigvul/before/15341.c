static zend_object_value xmlwriter_object_new(zend_class_entry *class_type TSRMLS_DC)
{
	ze_xmlwriter_object *intern;
	zend_object_value retval;

	intern = emalloc(sizeof(ze_xmlwriter_object));
	memset(&intern->zo, 0, sizeof(zend_object));
	intern->xmlwriter_ptr = NULL;
	
	zend_object_std_init(&intern->zo, class_type TSRMLS_CC);
	object_properties_init(&intern->zo, class_type);

	retval.handle = zend_objects_store_put(intern,
						NULL,
						(zend_objects_free_object_storage_t) xmlwriter_object_free_storage,
						NULL TSRMLS_CC);
	
	retval.handlers = (zend_object_handlers *) & xmlwriter_object_handlers;
	
	return retval;
}

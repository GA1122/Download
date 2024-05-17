static zend_object *php_zip_object_new(zend_class_entry *class_type)  
{
	ze_zip_object *intern;

	intern = ecalloc(1, sizeof(ze_zip_object) + zend_object_properties_size(class_type));
	intern->prop_handler = &zip_prop_handlers;
	zend_object_std_init(&intern->zo, class_type);
	object_properties_init(&intern->zo, class_type);
	intern->zo.handlers = &zip_object_handlers;

	return &intern->zo;
}
 

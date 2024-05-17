ZEND_API void zend_objects_proxy_clone(zend_proxy_object *object, zend_proxy_object **object_clone TSRMLS_DC)
{
	*object_clone = emalloc(sizeof(zend_proxy_object));
	(*object_clone)->object = object->object;
	(*object_clone)->property = object->property;
	zval_add_ref(&(*object_clone)->property);
	zval_add_ref(&(*object_clone)->object);
}

ZEND_API void zend_objects_proxy_free_storage(zend_proxy_object *object TSRMLS_DC)
{
	zval_ptr_dtor(&object->object);
	zval_ptr_dtor(&object->property);
	efree(object);
}

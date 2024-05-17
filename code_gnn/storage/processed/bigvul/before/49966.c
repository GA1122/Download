ZEND_API zval *zend_object_create_proxy(zval *object, zval *member TSRMLS_DC)
{
	zend_proxy_object *pobj = emalloc(sizeof(zend_proxy_object));
	zval *retval;

	pobj->object = object;
	zval_add_ref(&pobj->object);
	ALLOC_ZVAL(pobj->property);
	INIT_PZVAL_COPY(pobj->property, member);
	zval_copy_ctor(pobj->property);

	MAKE_STD_ZVAL(retval);
	Z_TYPE_P(retval) = IS_OBJECT;
	Z_OBJ_HANDLE_P(retval) = zend_objects_store_put(pobj, (zend_objects_store_dtor_t)zend_objects_proxy_destroy, (zend_objects_free_object_storage_t) zend_objects_proxy_free_storage, (zend_objects_store_clone_t) zend_objects_proxy_clone TSRMLS_CC);
	Z_OBJ_HT_P(retval) = &zend_object_proxy_handlers;

	return retval;
}

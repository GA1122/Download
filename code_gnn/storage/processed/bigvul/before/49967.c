ZEND_API zval* zend_object_proxy_get(zval *property TSRMLS_DC)
{
	zend_proxy_object *probj = zend_object_store_get_object(property TSRMLS_CC);

	if (Z_OBJ_HT_P(probj->object) && Z_OBJ_HT_P(probj->object)->read_property) {
		return Z_OBJ_HT_P(probj->object)->read_property(probj->object, probj->property, BP_VAR_R, 0 TSRMLS_CC);
	} else {
		zend_error(E_WARNING, "Cannot read property of object - no read handler defined");
	}

	return NULL;
}

ZEND_API void zend_objects_store_del_ref(zval *zobject TSRMLS_DC)
{
	zend_object_handle handle;

	handle = Z_OBJ_HANDLE_P(zobject);

	Z_ADDREF_P(zobject);
	zend_objects_store_del_ref_by_handle_ex(handle, Z_OBJ_HT_P(zobject) TSRMLS_CC);
	Z_DELREF_P(zobject);

	GC_ZOBJ_CHECK_POSSIBLE_ROOT(zobject);
}

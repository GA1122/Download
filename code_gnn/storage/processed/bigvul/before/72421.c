static void get_lazy_object(pdo_stmt_t *stmt, zval *return_value TSRMLS_DC)  
{
	if (Z_TYPE(stmt->lazy_object_ref) == IS_NULL) {
		Z_TYPE(stmt->lazy_object_ref) = IS_OBJECT;
		Z_OBJ_HANDLE(stmt->lazy_object_ref) = zend_objects_store_put(stmt, (zend_objects_store_dtor_t)zend_objects_destroy_object, (zend_objects_free_object_storage_t)pdo_row_free_storage, NULL TSRMLS_CC);
		Z_OBJ_HT(stmt->lazy_object_ref) = &pdo_row_object_handlers;
		stmt->refcount++;
	}
	Z_TYPE_P(return_value) = IS_OBJECT;
	Z_OBJ_HANDLE_P(return_value) = Z_OBJ_HANDLE(stmt->lazy_object_ref);
	Z_OBJ_HT_P(return_value) = Z_OBJ_HT(stmt->lazy_object_ref);
	zend_objects_store_add_ref(return_value TSRMLS_CC);
}
 
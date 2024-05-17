static zval *php_zip_get_property_ptr_ptr(zval *object, zval *member, int type, void **cache_slot)  
{
	ze_zip_object *obj;
	zval tmp_member;
	zval *retval = NULL;
	zip_prop_handler *hnd = NULL;
	zend_object_handlers *std_hnd;

	if (Z_TYPE_P(member) != IS_STRING) {
		ZVAL_COPY(&tmp_member, member);
		convert_to_string(&tmp_member);
		member = &tmp_member;
		cache_slot = NULL;
	}

	obj = Z_ZIP_P(object);

	if (obj->prop_handler != NULL) {
		hnd = zend_hash_find_ptr(obj->prop_handler, Z_STR_P(member));
	}

	if (hnd == NULL) {
		std_hnd = zend_get_std_object_handlers();
		retval = std_hnd->get_property_ptr_ptr(object, member, type, cache_slot);
	}

	if (member == &tmp_member) {
		zval_dtor(member);
	}

	return retval;
}
 

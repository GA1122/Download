static HashTable *php_zip_get_properties(zval *object) 
{
	ze_zip_object *obj;
	HashTable *props;
	zip_prop_handler *hnd;
	zend_string *key;

	obj = Z_ZIP_P(object);
	props = zend_std_get_properties(object);

	if (obj->prop_handler == NULL) {
		return NULL;
	}

	ZEND_HASH_FOREACH_STR_KEY_PTR(obj->prop_handler, key, hnd) {
		zval *ret, val;
		ret = php_zip_property_reader(obj, hnd, &val);
		if (ret == NULL) {
			ret = &EG(uninitialized_zval);
		}
		zend_hash_update(props, key, ret);
	} ZEND_HASH_FOREACH_END();

	return props;
}
 

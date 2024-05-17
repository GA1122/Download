SPL_METHOD(SplObjectStorage, offsetGet)
{
	zval *obj;
	spl_SplObjectStorageElement *element;
	spl_SplObjectStorage *intern = Z_SPLOBJSTORAGE_P(getThis());
	zend_string *hash;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "o", &obj) == FAILURE) {
		return;
	}

	hash = spl_object_storage_get_hash(intern, getThis(), obj);
	if (!hash) {
		return;
	}

	element = spl_object_storage_get(intern, hash);
	spl_object_storage_free_hash(intern, hash);

	if (!element) {
		zend_throw_exception_ex(spl_ce_UnexpectedValueException, 0, "Object not found");
	} else {
		zval *value = &element->inf;

		ZVAL_DEREF(value);
		ZVAL_COPY(return_value, value);
	}
}  

/* {{{ proto bool SplObjectStorage::addAll(SplObjectStorage $os)

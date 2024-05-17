SPL_METHOD(SplObjectStorage, removeAll)
{
	zval *obj;
	spl_SplObjectStorage *intern = Z_SPLOBJSTORAGE_P(getThis());
	spl_SplObjectStorage *other;
	spl_SplObjectStorageElement *element;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "O", &obj, spl_ce_SplObjectStorage) == FAILURE) {
		return;
	}

	other = Z_SPLOBJSTORAGE_P(obj);

	zend_hash_internal_pointer_reset(&other->storage);
	while ((element = zend_hash_get_current_data_ptr(&other->storage)) != NULL) {
		if (spl_object_storage_detach(intern, getThis(), &element->obj) == FAILURE) {
			zend_hash_move_forward(&other->storage);
		}
	}

	zend_hash_internal_pointer_reset_ex(&intern->storage, &intern->pos);
	intern->index = 0;

	RETURN_LONG(zend_hash_num_elements(&intern->storage));
}  

/* {{{ proto bool SplObjectStorage::removeAllExcept(SplObjectStorage $os)

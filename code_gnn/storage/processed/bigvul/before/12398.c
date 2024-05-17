SPL_METHOD(SplObjectStorage, attach)
{
	zval *obj, *inf = NULL;

	spl_SplObjectStorage *intern = (spl_SplObjectStorage*)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o|z!", &obj, &inf) == FAILURE) {
		return;
	}
	spl_object_storage_attach(intern, getThis(), obj, inf TSRMLS_CC);
}  

/* {{{ proto void SplObjectStorage::detach($obj)
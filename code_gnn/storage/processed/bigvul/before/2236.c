static zval *_xml_resource_zval(long value)
{
	zval *ret;
	TSRMLS_FETCH();

	MAKE_STD_ZVAL(ret);

	Z_TYPE_P(ret) = IS_RESOURCE;
	Z_LVAL_P(ret) = value;

	zend_list_addref(value);

	return ret;
}

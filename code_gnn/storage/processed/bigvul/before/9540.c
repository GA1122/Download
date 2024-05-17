 PHP_METHOD(snmp, getErrno)
 {
	php_snmp_object *snmp_object;
	zval *object = getThis();

	snmp_object = (php_snmp_object *)zend_object_store_get_object(object TSRMLS_CC);

	RETVAL_LONG(snmp_object->snmp_errno);
	return;
 }

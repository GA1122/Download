 PHP_METHOD(snmp, close)
 {
	php_snmp_object *snmp_object;
	zval *object = getThis();

	snmp_object = (php_snmp_object *)zend_object_store_get_object(object TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		RETURN_FALSE;
	}

	netsnmp_session_free(&(snmp_object->session));

	RETURN_TRUE;
}

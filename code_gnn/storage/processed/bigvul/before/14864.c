PHP_METHOD(SoapServer, setObject)
{
	soapServicePtr service;
	zval *obj;

	SOAP_SERVER_BEGIN_CODE();

	FETCH_THIS_SERVICE(service);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o", &obj) == FAILURE) {
		return;
	}

	service->type = SOAP_OBJECT;

	MAKE_STD_ZVAL(service->soap_object);
	MAKE_COPY_ZVAL(&obj, service->soap_object);

	SOAP_SERVER_END_CODE();
}

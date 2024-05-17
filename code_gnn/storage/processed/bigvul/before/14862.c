PHP_METHOD(SoapServer, setPersistence)
{
	soapServicePtr service;
	long value;

	SOAP_SERVER_BEGIN_CODE();

	FETCH_THIS_SERVICE(service);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &value) != FAILURE) {
		if (service->type == SOAP_CLASS) {
			if (value == SOAP_PERSISTENCE_SESSION ||
				value == SOAP_PERSISTENCE_REQUEST) {
				service->soap_class.persistance = value;
			} else {
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "Tried to set persistence with bogus value (%ld)", value);
				return;
			}
		} else {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Tried to set persistence when you are using you SOAP SERVER in function mode, no persistence needed");
			return;
		}
	}

	SOAP_SERVER_END_CODE();
}

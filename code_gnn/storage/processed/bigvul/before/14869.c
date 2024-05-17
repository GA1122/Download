PHP_METHOD(SoapServer, addSoapHeader)
{
	soapServicePtr service;
	zval *fault;
	soapHeader **p;

	SOAP_SERVER_BEGIN_CODE();

	FETCH_THIS_SERVICE(service);

	if (!service || !service->soap_headers_ptr) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "The SoapServer::addSoapHeader function may be called only during SOAP request processing");
		return;
	}

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &fault, soap_header_class_entry) == FAILURE) {
		return;
	}

	p = service->soap_headers_ptr;
	while (*p != NULL) {
		p = &(*p)->next;
	}
	*p = emalloc(sizeof(soapHeader));
	memset(*p, 0, sizeof(soapHeader));
	ZVAL_NULL(&(*p)->function_name);
	(*p)->retval = *fault;
	zval_copy_ctor(&(*p)->retval);

	SOAP_SERVER_END_CODE();
}

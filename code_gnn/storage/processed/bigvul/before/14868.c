PHP_METHOD(SoapServer, fault)
{
	char *code, *string, *actor=NULL, *name=NULL;
	int code_len, string_len, actor_len = 0, name_len = 0;
	zval* details = NULL;
	soapServicePtr service;
	xmlCharEncodingHandlerPtr old_encoding;

	SOAP_SERVER_BEGIN_CODE();
	FETCH_THIS_SERVICE(service);
	old_encoding = SOAP_GLOBAL(encoding);
	SOAP_GLOBAL(encoding) = service->encoding;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|szs",
	    &code, &code_len, &string, &string_len, &actor, &actor_len, &details,
	    &name, &name_len) == FAILURE) {
		return;
	}

	soap_server_fault(code, string, actor, details, name TSRMLS_CC);

	SOAP_GLOBAL(encoding) = old_encoding;
	SOAP_SERVER_END_CODE();
}

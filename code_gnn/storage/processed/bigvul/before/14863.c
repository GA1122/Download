PHP_METHOD(SoapServer, setClass)
{
	soapServicePtr service;
	char *classname;
	zend_class_entry **ce;

	int classname_len, found, num_args = 0;
	zval ***argv = NULL;

	SOAP_SERVER_BEGIN_CODE();

	FETCH_THIS_SERVICE(service);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s*", &classname, &classname_len, &argv, &num_args) == FAILURE) {
		return;
	}

	found = zend_lookup_class(classname, classname_len, &ce TSRMLS_CC);

	if (found != FAILURE) {
		service->type = SOAP_CLASS;
		service->soap_class.ce = *ce;

		service->soap_class.persistance = SOAP_PERSISTENCE_REQUEST;
		service->soap_class.argc = num_args;
		if (service->soap_class.argc > 0) {
			int i;
			service->soap_class.argv = safe_emalloc(sizeof(zval), service->soap_class.argc, 0);
			for (i = 0;i < service->soap_class.argc;i++) {
				service->soap_class.argv[i] = *(argv[i]);
				zval_add_ref(&service->soap_class.argv[i]);
			}
		}
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Tried to set a non existent class (%s)", classname);
		return;
	}

	if (argv) {
		efree(argv);
	}

	SOAP_SERVER_END_CODE();
}

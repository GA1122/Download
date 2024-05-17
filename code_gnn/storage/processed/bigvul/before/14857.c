PHP_METHOD(SoapHeader, SoapHeader)
{
	zval *data = NULL, *actor = NULL;
	char *name, *ns;
	int name_len, ns_len;
	zend_bool must_understand = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|zbz", &ns, &ns_len, &name, &name_len, &data, &must_understand, &actor) == FAILURE) {
		return;
	}
	if (ns_len == 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid namespace");
		return;
	}
	if (name_len == 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid header name");
		return;
	}

	add_property_stringl(this_ptr, "namespace", ns, ns_len, 1);
	add_property_stringl(this_ptr, "name", name, name_len, 1);
	if (data) {
		add_property_zval(this_ptr, "data", data);
	}
	add_property_bool(this_ptr, "mustUnderstand", must_understand);
	if (actor == NULL) {
	} else if (Z_TYPE_P(actor) == IS_LONG &&
	  (Z_LVAL_P(actor) == SOAP_ACTOR_NEXT ||
	   Z_LVAL_P(actor) == SOAP_ACTOR_NONE ||
	   Z_LVAL_P(actor) == SOAP_ACTOR_UNLIMATERECEIVER)) {
		add_property_long(this_ptr, "actor", Z_LVAL_P(actor));
	} else if (Z_TYPE_P(actor) == IS_STRING && Z_STRLEN_P(actor) > 0) {
		add_property_stringl(this_ptr, "actor", Z_STRVAL_P(actor), Z_STRLEN_P(actor), 1);
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid actor");
	}
}

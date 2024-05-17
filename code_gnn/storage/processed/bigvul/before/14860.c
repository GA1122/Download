PHP_METHOD(SoapVar, SoapVar)
{
	zval *data, *type;
	char *stype = NULL, *ns = NULL, *name = NULL, *namens = NULL;
	int stype_len = 0, ns_len = 0, name_len = 0, namens_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z!z|ssss", &data, &type, &stype, &stype_len, &ns, &ns_len, &name, &name_len, &namens, &namens_len) == FAILURE) {
		return;
	}

	if (Z_TYPE_P(type) == IS_NULL) {
		add_property_long(this_ptr, "enc_type", UNKNOWN_TYPE);
	} else {
		if (zend_hash_index_exists(&SOAP_GLOBAL(defEncIndex), Z_LVAL_P(type))) {
			add_property_long(this_ptr, "enc_type", Z_LVAL_P(type));
		} else {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid type ID");
			return;
		}
	}

	if (data) {
		add_property_zval(this_ptr, "enc_value", data);
	}

	if (stype && stype_len > 0) {
		add_property_stringl(this_ptr, "enc_stype", stype, stype_len, 1);
	}
	if (ns && ns_len > 0) {
		add_property_stringl(this_ptr, "enc_ns", ns, ns_len, 1);
	}
	if (name && name_len > 0) {
		add_property_stringl(this_ptr, "enc_name", name, name_len, 1);
	}
	if (namens && namens_len > 0) {
		add_property_stringl(this_ptr, "enc_namens", namens, namens_len, 1);
	}
}

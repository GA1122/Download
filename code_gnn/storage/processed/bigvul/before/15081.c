int dom_document_validate_on_parse_write(dom_object *obj, zval *newval TSRMLS_DC)
{
	zval value_copy;
	dom_doc_propsptr doc_prop;

	if(Z_REFCOUNT_P(newval) > 1) {
		value_copy = *newval;
		zval_copy_ctor(&value_copy);
		newval = &value_copy;
	}
	convert_to_boolean(newval);

	if (obj->document) {
		doc_prop = dom_get_doc_props(obj->document);
		doc_prop->validateonparse = Z_LVAL_P(newval);
	}

	if (newval == &value_copy) {
		zval_dtor(newval);
	}

	return SUCCESS;
}

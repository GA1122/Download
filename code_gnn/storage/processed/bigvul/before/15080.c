int	dom_document_validate_on_parse_read(dom_object *obj, zval **retval TSRMLS_DC)
{
	dom_doc_propsptr doc_prop;

	ALLOC_ZVAL(*retval);
	if (obj->document) {
		doc_prop = dom_get_doc_props(obj->document);
		ZVAL_BOOL(*retval, doc_prop->validateonparse);
	} else {
		ZVAL_FALSE(*retval);
	}
	return SUCCESS;
}
PHP_FUNCTION(dom_document_get_element_by_id)
{
	zval *id;
	xmlDocPtr docp;
	xmlAttrPtr  attrp;
	int ret, idname_len;
	dom_object *intern;
	char *idname;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &id, dom_document_class_entry, &idname, &idname_len) == FAILURE) {
		return;
	}

	DOM_GET_OBJ(docp, id, xmlDocPtr, intern);

	attrp = xmlGetID(docp, (xmlChar *) idname);

	if (attrp && attrp->parent) {
		DOM_RET_OBJ((xmlNodePtr) attrp->parent, &ret, intern);
	} else {
		RETVAL_NULL();
	}

}

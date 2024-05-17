PHP_FUNCTION(dom_document_create_element_ns)
{
	zval *id;
	xmlDocPtr docp;
	xmlNodePtr nodep = NULL;
	xmlNsPtr nsptr = NULL;
	int ret, uri_len = 0, name_len = 0, value_len = 0;
	char *uri, *name, *value = NULL;
	char *localname = NULL, *prefix = NULL;
	int errorcode;
	dom_object *intern;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os!s|s", &id, dom_document_class_entry, &uri, &uri_len, &name, &name_len, &value, &value_len) == FAILURE) {
		return;
	}

	DOM_GET_OBJ(docp, id, xmlDocPtr, intern);

	errorcode = dom_check_qname(name, &localname, &prefix, uri_len, name_len);

	if (errorcode == 0) {
		if (xmlValidateName((xmlChar *) localname, 0) == 0) {
			nodep = xmlNewDocNode (docp, NULL, localname, value);
			if (nodep != NULL && uri != NULL) {
				nsptr = xmlSearchNsByHref (nodep->doc, nodep, uri);
				if (nsptr == NULL) {
					nsptr = dom_get_ns(nodep, uri, &errorcode, prefix);
				}
				xmlSetNs(nodep, nsptr);
			}
		} else {
			errorcode = INVALID_CHARACTER_ERR;
		}
	}

	xmlFree(localname);
	if (prefix != NULL) {
		xmlFree(prefix);
	}

	if (errorcode != 0) {
		if (nodep != NULL) {
			xmlFreeNode(nodep);
		}
		php_dom_throw_error(errorcode, dom_get_strict_error(intern->document) TSRMLS_CC);
		RETURN_FALSE;
	}

	if (nodep == NULL) {
		RETURN_FALSE;
	}

	
	nodep->ns = nsptr;

	DOM_RET_OBJ(nodep, &ret, intern);
}

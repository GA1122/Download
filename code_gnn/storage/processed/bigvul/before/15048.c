PHP_FUNCTION(dom_document_create_attribute_ns)
{
	zval *id;
	xmlDocPtr docp;
	xmlNodePtr nodep = NULL, root;
	xmlNsPtr nsptr;
	int ret, uri_len = 0, name_len = 0;
	char *uri, *name;
	char *localname = NULL, *prefix = NULL;
	dom_object *intern;
	int errorcode;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os!s", &id, dom_document_class_entry, &uri, &uri_len, &name, &name_len) == FAILURE) {
		return;
	}

	DOM_GET_OBJ(docp, id, xmlDocPtr, intern);

	root = xmlDocGetRootElement(docp);
	if (root != NULL) {
		errorcode = dom_check_qname(name, &localname, &prefix, uri_len, name_len);
		if (errorcode == 0) {
			if (xmlValidateName((xmlChar *) localname, 0) == 0) {
				nodep = (xmlNodePtr) xmlNewDocProp(docp, localname, NULL);
				if (nodep != NULL && uri_len > 0) {
					nsptr = xmlSearchNsByHref (nodep->doc, root, uri);
					if (nsptr == NULL) {
						nsptr = dom_get_ns(root, uri, &errorcode, prefix);
					}
					xmlSetNs(nodep, nsptr);
				}
			} else {
				errorcode = INVALID_CHARACTER_ERR;
			}
		}
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Document Missing Root Element");
		RETURN_FALSE;
	}

	xmlFree(localname);
	if (prefix != NULL) {
		xmlFree(prefix);
	}

	if (errorcode != 0) {
		if (nodep != NULL) {
			xmlFreeProp((xmlAttrPtr) nodep);
		}
		php_dom_throw_error(errorcode, dom_get_strict_error(intern->document) TSRMLS_CC);
		RETURN_FALSE;
	}

	if (nodep == NULL) {
		RETURN_FALSE;
	}

	DOM_RET_OBJ(nodep, &ret, intern);
}

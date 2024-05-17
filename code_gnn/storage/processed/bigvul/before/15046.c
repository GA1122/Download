PHP_FUNCTION(dom_document_import_node)
{
	zval *id, *node;
	xmlDocPtr docp;
	xmlNodePtr nodep, retnodep;
	dom_object *intern, *nodeobj;
	int ret; 
	long recursive = 0;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "OO|l", &id, dom_document_class_entry, &node, dom_node_class_entry, &recursive) == FAILURE) {
		return;
	}

	DOM_GET_OBJ(docp, id, xmlDocPtr, intern);

	DOM_GET_OBJ(nodep, node, xmlNodePtr, nodeobj);

	if (nodep->type == XML_HTML_DOCUMENT_NODE || nodep->type == XML_DOCUMENT_NODE 
		|| nodep->type == XML_DOCUMENT_TYPE_NODE) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Cannot import: Node Type Not Supported");
		RETURN_FALSE;
	}

	if (nodep->doc == docp) {
		retnodep = nodep;
	} else {
		if ((recursive == 0) && (nodep->type == XML_ELEMENT_NODE)) {
			recursive = 2;
		}
		retnodep = xmlDocCopyNode(nodep, docp, recursive);
		if (!retnodep) {
			RETURN_FALSE;
		}

		if ((retnodep->type == XML_ATTRIBUTE_NODE) && (nodep->ns != NULL)) {
			xmlNsPtr nsptr = NULL;
			xmlNodePtr root = xmlDocGetRootElement(docp);

			nsptr = xmlSearchNsByHref (nodep->doc, root, nodep->ns->href);
			if (nsptr == NULL) {
				int errorcode;
				nsptr = dom_get_ns(root, (char *) nodep->ns->href, &errorcode, (char *) nodep->ns->prefix);
			}
			xmlSetNs(retnodep, nsptr);
		}
	}

	DOM_RET_OBJ((xmlNodePtr) retnodep, &ret, intern);
}

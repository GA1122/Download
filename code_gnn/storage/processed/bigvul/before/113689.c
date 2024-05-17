xsltKeyFunction(xmlXPathParserContextPtr ctxt, int nargs){    
    xmlXPathObjectPtr obj1, obj2;    
    
    if (nargs != 2) {
	xsltTransformError(xsltXPathGetTransformContext(ctxt), NULL, NULL,
		"key() : expects two arguments\n");
	ctxt->error = XPATH_INVALID_ARITY;
	return;
    }    

     
    obj2 = valuePop(ctxt);
    xmlXPathStringFunction(ctxt, 1);
    if ((obj2 == NULL) ||
	(ctxt->value == NULL) || (ctxt->value->type != XPATH_STRING)) {
	xsltTransformError(xsltXPathGetTransformContext(ctxt), NULL, NULL,
	    "key() : invalid arg expecting a string\n");
	ctxt->error = XPATH_INVALID_TYPE;
	xmlXPathFreeObject(obj2);

	return;
    }
     
    obj1 = valuePop(ctxt);    

    if ((obj2->type == XPATH_NODESET) || (obj2->type == XPATH_XSLT_TREE)) {
	int i;
	xmlXPathObjectPtr newobj, ret;

	ret = xmlXPathNewNodeSet(NULL);

	if (obj2->nodesetval != NULL) {
	    for (i = 0; i < obj2->nodesetval->nodeNr; i++) {
		valuePush(ctxt, xmlXPathObjectCopy(obj1));
		valuePush(ctxt,
			  xmlXPathNewNodeSet(obj2->nodesetval->nodeTab[i]));
		xmlXPathStringFunction(ctxt, 1);
		xsltKeyFunction(ctxt, 2);
		newobj = valuePop(ctxt);
		ret->nodesetval = xmlXPathNodeSetMerge(ret->nodesetval,
						       newobj->nodesetval);
		xmlXPathFreeObject(newobj);
	    }
	}
	valuePush(ctxt, ret);
    } else {
	xmlNodeSetPtr nodelist = NULL;
	xmlChar *key = NULL, *value;
	const xmlChar *keyURI;
	xsltTransformContextPtr tctxt;   
	xmlChar *qname, *prefix;
	xmlXPathContextPtr xpctxt = ctxt->context;
	xmlNodePtr tmpNode = NULL;
	xsltDocumentPtr oldDocInfo;

	tctxt = xsltXPathGetTransformContext(ctxt);

	oldDocInfo = tctxt->document;

	if (xpctxt->node == NULL) {
	    xsltTransformError(tctxt, NULL, tctxt->inst,
		"Internal error in xsltKeyFunction(): "
		"The context node is not set on the XPath context.\n");
	    tctxt->state = XSLT_STATE_STOPPED;
	    goto error;
	}	
	 
	qname = obj1->stringval;
	key = xmlSplitQName2(qname, &prefix);
	if (key == NULL) {
	    key = xmlStrdup(obj1->stringval);
	    keyURI = NULL;
	    if (prefix != NULL)
		xmlFree(prefix);
	} else {
	    if (prefix != NULL) {
		keyURI = xmlXPathNsLookup(xpctxt, prefix);
		if (keyURI == NULL) {
		    xsltTransformError(tctxt, NULL, tctxt->inst,
			"key() : prefix %s is not bound\n", prefix);
		     
		}
		xmlFree(prefix);
	    } else {
		keyURI = NULL;
	    }
	}

	 
	valuePush(ctxt, obj2);
	xmlXPathStringFunction(ctxt, 1);
	if ((ctxt->value == NULL) || (ctxt->value->type != XPATH_STRING)) {
	    xsltTransformError(tctxt, NULL, tctxt->inst,
		"key() : invalid arg expecting a string\n");
	    ctxt->error = XPATH_INVALID_TYPE;
	    goto error;
	}
	obj2 = valuePop(ctxt);
	value = obj2->stringval;
	
	 	
	if (xpctxt->node->type == XML_NAMESPACE_DECL) {
	     
	    if ((((xmlNsPtr) xpctxt->node)->next != NULL) &&
		(((xmlNsPtr) xpctxt->node)->next->type == XML_ELEMENT_NODE))
	    {
		tmpNode = (xmlNodePtr) ((xmlNsPtr) xpctxt->node)->next;
	    }
	} else
	    tmpNode = xpctxt->node;

	if ((tmpNode == NULL) || (tmpNode->doc == NULL)) {
	    xsltTransformError(tctxt, NULL, tctxt->inst,
		"Internal error in xsltKeyFunction(): "
		"Couldn't get the doc of the XPath context node.\n");
	    goto error;
	}

	if ((tctxt->document == NULL) ||
	    (tctxt->document->doc != tmpNode->doc))
	{	   
	    if (tmpNode->doc->name && (tmpNode->doc->name[0] == ' ')) {
		 
		if (tmpNode->doc->_private == NULL) {
		    tmpNode->doc->_private = xsltNewDocument(tctxt, tmpNode->doc);
		    if (tmpNode->doc->_private == NULL)
			goto error;
		}
		tctxt->document = (xsltDocumentPtr) tmpNode->doc->_private;		
	    } else {
		 
		tctxt->document = xsltFindDocument(tctxt, tmpNode->doc);
	    }
	    if (tctxt->document == NULL) {
		xsltTransformError(tctxt, NULL, tctxt->inst,
		    "Internal error in xsltKeyFunction(): "
		    "Could not get the document info of a context doc.\n");
		tctxt->state = XSLT_STATE_STOPPED;
		goto error;
	    }
	}
	 
	nodelist = xsltGetKey(tctxt, key, keyURI, value);

error:	
	tctxt->document = oldDocInfo;
	valuePush(ctxt, xmlXPathWrapNodeSet(
	    xmlXPathNodeSetMerge(NULL, nodelist)));
	if (key != NULL)
	    xmlFree(key);
    }    

    if (obj1 != NULL)
	xmlXPathFreeObject(obj1);
    if (obj2 != NULL)
	xmlXPathFreeObject(obj2);    
}
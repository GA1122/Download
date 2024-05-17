exsltStrTokenizeFunction(xmlXPathParserContextPtr ctxt, int nargs)
{
    xsltTransformContextPtr tctxt;
    xmlChar *str, *delimiters, *cur;
    const xmlChar *token, *delimiter;
    xmlNodePtr node;
    xmlDocPtr container;
    xmlXPathObjectPtr ret = NULL;
    int clen;

    if ((nargs < 1) || (nargs > 2)) {
        xmlXPathSetArityError(ctxt);
        return;
    }

    if (nargs == 2) {
        delimiters = xmlXPathPopString(ctxt);
        if (xmlXPathCheckError(ctxt))
            return;
    } else {
        delimiters = xmlStrdup((const xmlChar *) "\t\r\n ");
    }
    if (delimiters == NULL)
        return;

    str = xmlXPathPopString(ctxt);
    if (xmlXPathCheckError(ctxt) || (str == NULL)) {
        xmlFree(delimiters);
        return;
    }

     
    tctxt = xsltXPathGetTransformContext(ctxt);
    if (tctxt == NULL) {
        xsltTransformError(xsltXPathGetTransformContext(ctxt), NULL, NULL,
	      "exslt:tokenize : internal error tctxt == NULL\n");
	goto fail;
    }

    container = xsltCreateRVT(tctxt);
    if (container != NULL) {
        xsltRegisterLocalRVT(tctxt, container);
        ret = xmlXPathNewNodeSet(NULL);
        if (ret != NULL) {
            for (cur = str, token = str; *cur != 0; cur += clen) {
	        clen = xmlUTF8Size(cur);
		if (*delimiters == 0) {	 
		    xmlChar ctmp;
		    ctmp = *(cur+clen);
		    *(cur+clen) = 0;
                    node = xmlNewDocRawNode(container, NULL,
                                       (const xmlChar *) "token", cur);
		    xmlAddChild((xmlNodePtr) container, node);
		    xmlXPathNodeSetAddUnique(ret->nodesetval, node);
                    *(cur+clen) = ctmp;  
                    token = cur + clen;
                } else for (delimiter = delimiters; *delimiter != 0;
				delimiter += xmlUTF8Size(delimiter)) {
                    if (!xmlUTF8Charcmp(cur, delimiter)) {
                        if (cur == token) {
                             
                            token = cur + clen;
                            break;
                        }
                        *cur = 0;	 
                        node = xmlNewDocRawNode(container, NULL,
                                           (const xmlChar *) "token", token);
			xmlAddChild((xmlNodePtr) container, node);
			xmlXPathNodeSetAddUnique(ret->nodesetval, node);
                        *cur = *delimiter;  
                        token = cur + clen;
                        break;
                    }
                }
            }
            if (token != cur) {
		node = xmlNewDocRawNode(container, NULL,
				    (const xmlChar *) "token", token);
                xmlAddChild((xmlNodePtr) container, node);
	        xmlXPathNodeSetAddUnique(ret->nodesetval, node);
            }
	     
	    xsltExtensionInstructionResultRegister(tctxt, ret);
        }
    }

fail:
    if (str != NULL)
        xmlFree(str);
    if (delimiters != NULL)
        xmlFree(delimiters);
    if (ret != NULL)
        valuePush(ctxt, ret);
    else
        valuePush(ctxt, xmlXPathNewNodeSet(NULL));
}

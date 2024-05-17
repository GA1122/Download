htmlParseContent(htmlParserCtxtPtr ctxt) {
    xmlChar *currentNode;
    int depth;
    const xmlChar *name;

    currentNode = xmlStrdup(ctxt->name);
    depth = ctxt->nameNr;
    while (1) {
	long cons = ctxt->nbChars;

        GROW;

        if (ctxt->instate == XML_PARSER_EOF)
            break;

	 
        if ((CUR == '<') && (NXT(1) == '/')) {
	    if (htmlParseEndTag(ctxt) &&
		((currentNode != NULL) || (ctxt->nameNr == 0))) {
		if (currentNode != NULL)
		    xmlFree(currentNode);
		return;
	    }
	    continue;  
        }

	else if ((CUR == '<') &&
	         ((IS_ASCII_LETTER(NXT(1))) ||
		  (NXT(1) == '_') || (NXT(1) == ':'))) {
	    name = htmlParseHTMLName_nonInvasive(ctxt);
	    if (name == NULL) {
	        htmlParseErr(ctxt, XML_ERR_NAME_REQUIRED,
			 "htmlParseStartTag: invalid element name\n",
			 NULL, NULL);
	         
        while ((IS_CHAR_CH(CUR)) && (CUR != '>'))
	            NEXT;

	        if (currentNode != NULL)
	            xmlFree(currentNode);
	        return;
	    }

	    if (ctxt->name != NULL) {
	        if (htmlCheckAutoClose(name, ctxt->name) == 1) {
	            htmlAutoClose(ctxt, name);
	            continue;
	        }
	    }
	}

	 
        if ((ctxt->nameNr > 0) && (depth >= ctxt->nameNr) &&
	    (!xmlStrEqual(currentNode, ctxt->name)))
	     {
	    if (currentNode != NULL) xmlFree(currentNode);
	    return;
	}

	if ((CUR != 0) && ((xmlStrEqual(currentNode, BAD_CAST"script")) ||
	    (xmlStrEqual(currentNode, BAD_CAST"style")))) {
	     
	    htmlParseScript(ctxt);
	} else {
	     
	    if ((CUR == '<') && (NXT(1) == '!') &&
		(UPP(2) == 'D') && (UPP(3) == 'O') &&
		(UPP(4) == 'C') && (UPP(5) == 'T') &&
		(UPP(6) == 'Y') && (UPP(7) == 'P') &&
		(UPP(8) == 'E')) {
		htmlParseErr(ctxt, XML_HTML_STRUCURE_ERROR,
		             "Misplaced DOCTYPE declaration\n",
			     BAD_CAST "DOCTYPE" , NULL);
		htmlParseDocTypeDecl(ctxt);
	    }

	     
	    if ((CUR == '<') && (NXT(1) == '!') &&
		(NXT(2) == '-') && (NXT(3) == '-')) {
		htmlParseComment(ctxt);
	    }

	     
	    else if ((CUR == '<') && (NXT(1) == '?')) {
		htmlParsePI(ctxt);
	    }

	     
	    else if (CUR == '<') {
		htmlParseElement(ctxt);
	    }

	     
	    else if (CUR == '&') {
		htmlParseReference(ctxt);
	    }

	     
	    else if (CUR == 0) {
		htmlAutoCloseOnEnd(ctxt);
		break;
	    }

	     
	    else {
		htmlParseCharData(ctxt);
	    }

	    if (cons == ctxt->nbChars) {
		if (ctxt->node != NULL) {
		    htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
		                 "detected an error in element content\n",
				 NULL, NULL);
		}
		break;
	    }
	}
        GROW;
    }
    if (currentNode != NULL) xmlFree(currentNode);
}

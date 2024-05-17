htmlParseStartTag(htmlParserCtxtPtr ctxt) {
    const xmlChar *name;
    const xmlChar *attname;
    xmlChar *attvalue;
    const xmlChar **atts;
    int nbatts = 0;
    int maxatts;
    int meta = 0;
    int i;
    int discardtag = 0;

    if ((ctxt == NULL) || (ctxt->input == NULL)) {
	htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
		     "htmlParseStartTag: context error\n", NULL, NULL);
	return -1;
    }
    if (ctxt->instate == XML_PARSER_EOF)
        return(-1);
    if (CUR != '<') return -1;
    NEXT;

    atts = ctxt->atts;
    maxatts = ctxt->maxatts;

    GROW;
    name = htmlParseHTMLName(ctxt);
    if (name == NULL) {
	htmlParseErr(ctxt, XML_ERR_NAME_REQUIRED,
	             "htmlParseStartTag: invalid element name\n",
		     NULL, NULL);
	 
	if ((ctxt->recovery) && ((IS_BLANK_CH(CUR)) || (CUR == '<') ||
	    (CUR == '=') || (CUR == '>') || (((CUR >= '0') && (CUR <= '9'))))) {
	    htmlParseCharDataInternal(ctxt, '<');
	    return(-1);
	}


	 
	while ((IS_CHAR_CH(CUR)) && (CUR != '>') &&
               (ctxt->instate != XML_PARSER_EOF))
	    NEXT;
        return -1;
    }
    if (xmlStrEqual(name, BAD_CAST"meta"))
	meta = 1;

     
    htmlAutoClose(ctxt, name);

     
    htmlCheckImplied(ctxt, name);

     
    if ((ctxt->nameNr > 0) && (xmlStrEqual(name, BAD_CAST"html"))) {
	htmlParseErr(ctxt, XML_HTML_STRUCURE_ERROR,
	             "htmlParseStartTag: misplaced <html> tag\n",
		     name, NULL);
	discardtag = 1;
	ctxt->depth++;
    }
    if ((ctxt->nameNr != 1) &&
	(xmlStrEqual(name, BAD_CAST"head"))) {
	htmlParseErr(ctxt, XML_HTML_STRUCURE_ERROR,
	             "htmlParseStartTag: misplaced <head> tag\n",
		     name, NULL);
	discardtag = 1;
	ctxt->depth++;
    }
    if (xmlStrEqual(name, BAD_CAST"body")) {
	int indx;
	for (indx = 0;indx < ctxt->nameNr;indx++) {
	    if (xmlStrEqual(ctxt->nameTab[indx], BAD_CAST"body")) {
		htmlParseErr(ctxt, XML_HTML_STRUCURE_ERROR,
		             "htmlParseStartTag: misplaced <body> tag\n",
			     name, NULL);
		discardtag = 1;
		ctxt->depth++;
	    }
	}
    }

     
    SKIP_BLANKS;
    while ((IS_CHAR_CH(CUR)) &&
           (CUR != '>') &&
	   ((CUR != '/') || (NXT(1) != '>'))) {
	long cons = ctxt->nbChars;

	GROW;
	attname = htmlParseAttribute(ctxt, &attvalue);
        if (attname != NULL) {

	     
	    for (i = 0; i < nbatts;i += 2) {
	        if (xmlStrEqual(atts[i], attname)) {
		    htmlParseErr(ctxt, XML_ERR_ATTRIBUTE_REDEFINED,
		                 "Attribute %s redefined\n", attname, NULL);
		    if (attvalue != NULL)
			xmlFree(attvalue);
		    goto failed;
		}
	    }

	     
	    if (atts == NULL) {
	        maxatts = 22;  
	        atts = (const xmlChar **)
		       xmlMalloc(maxatts * sizeof(xmlChar *));
		if (atts == NULL) {
		    htmlErrMemory(ctxt, NULL);
		    if (attvalue != NULL)
			xmlFree(attvalue);
		    goto failed;
		}
		ctxt->atts = atts;
		ctxt->maxatts = maxatts;
	    } else if (nbatts + 4 > maxatts) {
	        const xmlChar **n;

	        maxatts *= 2;
	        n = (const xmlChar **) xmlRealloc((void *) atts,
					     maxatts * sizeof(const xmlChar *));
		if (n == NULL) {
		    htmlErrMemory(ctxt, NULL);
		    if (attvalue != NULL)
			xmlFree(attvalue);
		    goto failed;
		}
		atts = n;
		ctxt->atts = atts;
		ctxt->maxatts = maxatts;
	    }
	    atts[nbatts++] = attname;
	    atts[nbatts++] = attvalue;
	    atts[nbatts] = NULL;
	    atts[nbatts + 1] = NULL;
	}
	else {
	    if (attvalue != NULL)
	        xmlFree(attvalue);
	     
	    while ((IS_CHAR_CH(CUR)) &&
	           !(IS_BLANK_CH(CUR)) && (CUR != '>') &&
		   ((CUR != '/') || (NXT(1) != '>')))
		NEXT;
	}

failed:
	SKIP_BLANKS;
        if (cons == ctxt->nbChars) {
	    htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
	                 "htmlParseStartTag: problem parsing attributes\n",
			 NULL, NULL);
	    break;
	}
    }

     
    if (meta && (nbatts != 0))
	htmlCheckMeta(ctxt, atts);

     
    if (!discardtag) {
	htmlnamePush(ctxt, name);
	if ((ctxt->sax != NULL) && (ctxt->sax->startElement != NULL)) {
	    if (nbatts != 0)
		ctxt->sax->startElement(ctxt->userData, name, atts);
	    else
		ctxt->sax->startElement(ctxt->userData, name, NULL);
	}
    }

    if (atts != NULL) {
        for (i = 1;i < nbatts;i += 2) {
	    if (atts[i] != NULL)
		xmlFree((xmlChar *) atts[i]);
	}
    }

    return(discardtag);
}
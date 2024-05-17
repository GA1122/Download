xmlParseAttribute(xmlParserCtxtPtr ctxt, xmlChar **value) {
    const xmlChar *name;
    xmlChar *val;

    *value = NULL;
    GROW;
    name = xmlParseName(ctxt);
    if (name == NULL) {
	xmlFatalErrMsg(ctxt, XML_ERR_NAME_REQUIRED,
	               "error parsing attribute name\n");
        return(NULL);
    }

     
    SKIP_BLANKS;
    if (RAW == '=') {
        NEXT;
	SKIP_BLANKS;
	val = xmlParseAttValue(ctxt);
	ctxt->instate = XML_PARSER_CONTENT;
    } else {
	xmlFatalErrMsgStr(ctxt, XML_ERR_ATTRIBUTE_WITHOUT_VALUE,
	       "Specification mandate value for attribute %s\n", name);
	return(NULL);
    }

     
    if ((ctxt->pedantic) && (xmlStrEqual(name, BAD_CAST "xml:lang"))) {
	if (!xmlCheckLanguageID(val)) {
	    xmlWarningMsg(ctxt, XML_WAR_LANG_VALUE,
		          "Malformed value for xml:lang : %s\n",
			  val, NULL);
	}
    }

     
    if (xmlStrEqual(name, BAD_CAST "xml:space")) {
	if (xmlStrEqual(val, BAD_CAST "default"))
	    *(ctxt->space) = 0;
	else if (xmlStrEqual(val, BAD_CAST "preserve"))
	    *(ctxt->space) = 1;
	else {
		xmlWarningMsg(ctxt, XML_WAR_SPACE_VALUE,
"Invalid value \"%s\" for xml:space : \"default\" or \"preserve\" expected\n",
                                 val, NULL);
	}
    }

    *value = val;
    return(name);
}
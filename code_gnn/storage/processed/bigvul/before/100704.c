xmlParseAttribute2(xmlParserCtxtPtr ctxt,
                   const xmlChar * pref, const xmlChar * elem,
                   const xmlChar ** prefix, xmlChar ** value,
                   int *len, int *alloc)
{
    const xmlChar *name;
    xmlChar *val, *internal_val = NULL;
    int normalize = 0;

    *value = NULL;
    GROW;
    name = xmlParseQName(ctxt, prefix);
    if (name == NULL) {
        xmlFatalErrMsg(ctxt, XML_ERR_NAME_REQUIRED,
                       "error parsing attribute name\n");
        return (NULL);
    }

     
    if (ctxt->attsSpecial != NULL) {
        int type;

        type = (int) (long) xmlHashQLookup2(ctxt->attsSpecial,
                                            pref, elem, *prefix, name);
        if (type != 0)
            normalize = 1;
    }

     
    SKIP_BLANKS;
    if (RAW == '=') {
        NEXT;
        SKIP_BLANKS;
        val = xmlParseAttValueInternal(ctxt, len, alloc, normalize);
	if (normalize) {
	     
	    if (*alloc) {
	        const xmlChar *val2;

	        val2 = xmlAttrNormalizeSpace2(ctxt, val, len);
		if ((val2 != NULL) && (val2 != val)) {
		    xmlFree(val);
		    val = (xmlChar *) val2;
		}
	    }
	}
        ctxt->instate = XML_PARSER_CONTENT;
    } else {
        xmlFatalErrMsgStr(ctxt, XML_ERR_ATTRIBUTE_WITHOUT_VALUE,
                          "Specification mandate value for attribute %s\n",
                          name);
        return (NULL);
    }

    if (*prefix == ctxt->str_xml) {
         
        if ((ctxt->pedantic) && (xmlStrEqual(name, BAD_CAST "lang"))) {
            internal_val = xmlStrndup(val, *len);
            if (!xmlCheckLanguageID(internal_val)) {
                xmlWarningMsg(ctxt, XML_WAR_LANG_VALUE,
                              "Malformed value for xml:lang : %s\n",
                              internal_val, NULL);
            }
        }

         
        if (xmlStrEqual(name, BAD_CAST "space")) {
            internal_val = xmlStrndup(val, *len);
            if (xmlStrEqual(internal_val, BAD_CAST "default"))
                *(ctxt->space) = 0;
            else if (xmlStrEqual(internal_val, BAD_CAST "preserve"))
                *(ctxt->space) = 1;
            else {
                xmlWarningMsg(ctxt, XML_WAR_SPACE_VALUE,
                              "Invalid value \"%s\" for xml:space : \"default\" or \"preserve\" expected\n",
                              internal_val, NULL);
            }
        }
        if (internal_val) {
            xmlFree(internal_val);
        }
    }

    *value = val;
    return (name);
}
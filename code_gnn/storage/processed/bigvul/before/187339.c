 xsltGetQNameProperty(xsltStylesheetPtr style, xmlNodePtr inst,
 		     const xmlChar *propName,
 		     int mandatory,
 		     int *hasProp, const xmlChar **nsName,
 		     const xmlChar** localName)
 {
     const xmlChar *prop;
 
     if (nsName)
 	*nsName = NULL;
     if (localName)
 	*localName = NULL;
     if (hasProp)
 	*hasProp = 0;
 
     prop = xsltGetCNsProp(style, inst, propName, XSLT_NAMESPACE);
     if (prop == NULL) {
 	if (mandatory) {
 	    xsltTransformError(NULL, style, inst,
 		"The attribute '%s' is missing.\n", propName);
 	    style->errors++;
 	    return;
 	}
     } else {
         const xmlChar *URI;
 
 	if (xmlValidateQName(prop, 0)) {
 	    xsltTransformError(NULL, style, inst,
 		"The value '%s' of the attribute "
 		"'%s' is not a valid QName.\n", prop, propName);
 	    style->errors++;
 	    return;
 	} else {
 	     
 	    URI = xsltGetQNameURI2(style, inst, &prop);
  	    if (prop == NULL) {
  		style->errors++;
  	    } else {
		*localName = prop;
// 		if (localName)
// 		    *localName = prop;
  		if (hasProp)
  		    *hasProp = 1;
  		if (URI != NULL) {
 		     
 		    if (nsName)
 			*nsName = xmlDictLookup(style->dict, URI, -1);
 		     
 		}
 	    }
 	}
     }
     return;
 }
 xsltAttributeComp(xsltStylesheetPtr style, xmlNodePtr inst) {
 #ifdef XSLT_REFACTORED
     xsltStyleItemAttributePtr comp;
 #else
     xsltStylePreCompPtr comp;
 #endif
 
      
     if ((style == NULL) || (inst == NULL) || (inst->type != XML_ELEMENT_NODE))
 	return;
 
 #ifdef XSLT_REFACTORED
     comp = (xsltStyleItemAttributePtr) xsltNewStylePreComp(style,
 	XSLT_FUNC_ATTRIBUTE);
 #else
     comp = xsltNewStylePreComp(style, XSLT_FUNC_ATTRIBUTE);
 #endif
 
     if (comp == NULL)
 	return;
     inst->psvi = comp;
     comp->inst = inst;
 
      
      
     comp->name = xsltEvalStaticAttrValueTemplate(style, inst,
 				 (const xmlChar *)"name",
 				 NULL, &comp->has_name);
     if (! comp->has_name) {
 	xsltTransformError(NULL, style, inst,
 	    "XSLT-attribute: The attribute 'name' is missing.\n");
 	style->errors++;
 	return;
     }
      
      
     comp->ns = xsltEvalStaticAttrValueTemplate(style, inst,
 	(const xmlChar *)"namespace",
 	NULL, &comp->has_ns);
 
     if (comp->name != NULL) {
 	if (xmlValidateQName(comp->name, 0)) {
 	    xsltTransformError(NULL, style, inst,
 		"xsl:attribute: The value '%s' of the attribute 'name' is "
 		"not a valid QName.\n", comp->name);
 	    style->errors++;
         } else if (xmlStrEqual(comp->name, BAD_CAST "xmlns")) {
 	    xsltTransformError(NULL, style, inst,
                 "xsl:attribute: The attribute name 'xmlns' is not allowed.\n");
 	    style->errors++;
 	} else {
 	    const xmlChar *prefix = NULL, *name;
 
 	    name = xsltSplitQName(style->dict, comp->name, &prefix);
 	    if (prefix != NULL) {
 		if (comp->has_ns == 0) {
 		    xmlNsPtr ns;
 
 		     
 		    ns = xmlSearchNs(inst->doc, inst, prefix);
 		    if (ns != NULL) {
 			comp->ns = xmlDictLookup(style->dict, ns->href, -1);
 			comp->has_ns = 1;
  #ifdef XSLT_REFACTORED
  			comp->nsPrefix = prefix;
  			comp->name = name;
// #else
//                         (void)name;  
  #endif
  		    } else {
  			xsltTransformError(NULL, style, inst,
 			    "xsl:attribute: The prefixed QName '%s' "
 			    "has no namespace binding in scope in the "
 			    "stylesheet; this is an error, since the "
 			    "namespace was not specified by the instruction "
 			    "itself.\n", comp->name);
 			style->errors++;
 		    }
 		}
 	    }
 	}
     }
 }
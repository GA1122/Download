 xsltAttrTemplateProcess(xsltTransformContextPtr ctxt, xmlNodePtr target,
 	                xmlAttrPtr attr)
 {
     const xmlChar *value;
     xmlAttrPtr ret;
 
     if ((ctxt == NULL) || (attr == NULL) || (target == NULL))
 	return(NULL);
     
     if (attr->type != XML_ATTRIBUTE_NODE)
 	return(NULL);
 
      
 #ifdef XSLT_REFACTORED    
     if (attr->psvi == xsltXSLTAttrMarker)
 	return(NULL);
 #else
     if ((attr->ns != NULL) && xmlStrEqual(attr->ns->href, XSLT_NAMESPACE))
 	return(NULL);
 #endif
      
     if (attr->children != NULL) {
 	if ((attr->children->type != XML_TEXT_NODE) ||
 	    (attr->children->next != NULL))
 	{
 	    xsltTransformError(ctxt, NULL, attr->parent,
 		"Internal error: The children of an attribute node of a "
 		"literal result element are not in the expected form.\n");
 	    return(NULL);
 	}
 	value = attr->children->content;
 	if (value == NULL)
 	    value = xmlDictLookup(ctxt->dict, BAD_CAST "", 0);
     } else
 	value = xmlDictLookup(ctxt->dict, BAD_CAST "", 0);
      
     ret = target->properties;
     while (ret != NULL) {
         if (((attr->ns != NULL) == (ret->ns != NULL)) &&
 	    xmlStrEqual(ret->name, attr->name) &&
 	    ((attr->ns == NULL) || xmlStrEqual(ret->ns->href, attr->ns->href)))
 	{
 	    break;
 	}
         ret = ret->next;
     }
     if (ret != NULL) {	
          
 	xmlFreeNodeList(ret->children);
 	ret->children = ret->last = NULL;
 	 
 	if ((ret->ns != NULL) &&
 	    (! xmlStrEqual(ret->ns->prefix, attr->ns->prefix)))
 	{
 	    ret->ns = xsltGetNamespace(ctxt, attr->parent, attr->ns, target);
 	}
     } else {
          
 	if (attr->ns != NULL)
 	    ret = xmlNewNsProp(target,
 		xsltGetNamespace(ctxt, attr->parent, attr->ns, target),
 		    attr->name, NULL);
 	else
 	    ret = xmlNewNsProp(target, NULL, attr->name, NULL);	
     }
      
     if (ret != NULL) {
         xmlNodePtr text;
 
         text = xmlNewText(NULL);
 	if (text != NULL) {
 	    ret->last = ret->children = text;
 	    text->parent = (xmlNodePtr) ret;
 	    text->doc = ret->doc;
 
 	    if (attr->psvi != NULL) {
 		 
 		xmlChar *val;
 		val = xsltEvalAVT(ctxt, attr->psvi, attr->parent);
 		if (val == NULL) {
 		     
 		    if (attr->ns) {
 			xsltTransformError(ctxt, NULL, attr->parent,
 			    "Internal error: Failed to evaluate the AVT "
 			    "of attribute '{%s}%s'.\n",
 			    attr->ns->href, attr->name);
 		    } else {
 			xsltTransformError(ctxt, NULL, attr->parent,
 			    "Internal error: Failed to evaluate the AVT "
 			    "of attribute '%s'.\n",
 			    attr->name);
 		    }
 		    text->content = xmlStrdup(BAD_CAST "");
 		} else {
 		    text->content = val;
  		}
  	    } else if ((ctxt->internalized) && (target != NULL) &&
  	               (target->doc != NULL) &&
		       (target->doc->dict == ctxt->dict)) {
// 		       (target->doc->dict == ctxt->dict) &&
// 		       xmlDictOwns(ctxt->dict, value)) {
  		text->content = (xmlChar *) value;
  	    } else {
  		text->content = xmlStrdup(value);
 	    }
 	}
     } else {
 	if (attr->ns) {
 	    xsltTransformError(ctxt, NULL, attr->parent,
 	    	"Internal error: Failed to create attribute '{%s}%s'.\n",
 		attr->ns->href, attr->name);
 	} else {
 	    xsltTransformError(ctxt, NULL, attr->parent,
 	    	"Internal error: Failed to create attribute '%s'.\n",
 		attr->name);
 	}
     }
     return(ret);
 }
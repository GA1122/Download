 xsltShallowCopyElem(xsltTransformContextPtr ctxt, xmlNodePtr node,
 		    xmlNodePtr insert, int isLRE)
 {
     xmlNodePtr copy;
 
     if ((node->type == XML_DTD_NODE) || (insert == NULL))
 	return(NULL);
     if ((node->type == XML_TEXT_NODE) ||
 	(node->type == XML_CDATA_SECTION_NODE))
 	return(xsltCopyText(ctxt, insert, node, 0));
 
     copy = xmlDocCopyNode(node, insert->doc, 0);
      if (copy != NULL) {
  	copy->doc = ctxt->output;
  	copy = xsltAddChild(insert, copy);
//         if (copy == NULL) {
//              xsltTransformError(ctxt, NULL, node,
//                 "xsltShallowCopyElem: copy failed\n");
//              return (copy);
//         }
  
  	if (node->type == XML_ELEMENT_NODE) {
  	     
 	    if (node->nsDef != NULL) {
 		 
 		if (isLRE)
 		    xsltCopyNamespaceList(ctxt, copy, node->nsDef);
 		else
 		    xsltCopyNamespaceListInternal(copy, node->nsDef);
 	    }
 
 	     
 	    if (node->ns != NULL) {
 		if (isLRE) {
 		     
 		    copy->ns = xsltGetNamespace(ctxt, node, node->ns, copy);
 		} else {
 		    copy->ns = xsltGetSpecialNamespace(ctxt,
 			node, node->ns->href, node->ns->prefix, copy);
 
 		}
 	    } else if ((insert->type == XML_ELEMENT_NODE) &&
 		       (insert->ns != NULL))
 	    {
 		 
 		xsltGetSpecialNamespace(ctxt, node, NULL, NULL, copy);
 	    }
 	}
     } else {
 	xsltTransformError(ctxt, NULL, node,
 		"xsltShallowCopyElem: copy %s failed\n", node->name);
     }
     return(copy);
 }
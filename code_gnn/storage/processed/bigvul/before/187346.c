 xsltCopyTreeInternal(xsltTransformContextPtr ctxt,
 		     xmlNodePtr invocNode,
 		     xmlNodePtr node,
 		     xmlNodePtr insert, int isLRE, int topElemVisited)
 {
     xmlNodePtr copy;
 
     if (node == NULL)
 	return(NULL);
     switch (node->type) {
         case XML_ELEMENT_NODE:
         case XML_ENTITY_REF_NODE:
         case XML_ENTITY_NODE:
         case XML_PI_NODE:
         case XML_COMMENT_NODE:
         case XML_DOCUMENT_NODE:
         case XML_HTML_DOCUMENT_NODE:
 #ifdef LIBXML_DOCB_ENABLED
         case XML_DOCB_DOCUMENT_NODE:
 #endif
 	    break;
         case XML_TEXT_NODE: {
 	    int noenc = (node->name == xmlStringTextNoenc);
 	    return(xsltCopyTextString(ctxt, insert, node->content, noenc));
 	    }
         case XML_CDATA_SECTION_NODE:
 	    return(xsltCopyTextString(ctxt, insert, node->content, 0));
         case XML_ATTRIBUTE_NODE:
 	    return((xmlNodePtr)
 		xsltShallowCopyAttr(ctxt, invocNode, insert, (xmlAttrPtr) node));
         case XML_NAMESPACE_DECL:
 	    return((xmlNodePtr) xsltShallowCopyNsNode(ctxt, invocNode,
 		insert, (xmlNsPtr) node));
 
         case XML_DOCUMENT_TYPE_NODE:
         case XML_DOCUMENT_FRAG_NODE:
         case XML_NOTATION_NODE:
         case XML_DTD_NODE:
         case XML_ELEMENT_DECL:
         case XML_ATTRIBUTE_DECL:
         case XML_ENTITY_DECL:
         case XML_XINCLUDE_START:
         case XML_XINCLUDE_END:
             return(NULL);
     }
     if (XSLT_IS_RES_TREE_FRAG(node)) {
 	if (node->children != NULL)
 	    copy = xsltCopyTreeList(ctxt, invocNode,
 		node->children, insert, 0, 0);
 	else
 	    copy = NULL;
 	return(copy);
     }
     copy = xmlDocCopyNode(node, insert->doc, 0);
      if (copy != NULL) {
  	copy->doc = ctxt->output;
  	copy = xsltAddChild(insert, copy);
//         if (copy == NULL) {
//             xsltTransformError(ctxt, NULL, invocNode,
//             "xsltCopyTreeInternal: Copying of '%s' failed.\n", node->name);
//             return (copy);
//         }
  	 
 	if (insert->last != copy)
 	    return(insert->last);
 	copy->next = NULL;
 
 	if (node->type == XML_ELEMENT_NODE) {
 	     
 	    if ((topElemVisited == 0) &&
 		(node->parent != NULL) &&
 		(node->parent->type != XML_DOCUMENT_NODE) &&
 		(node->parent->type != XML_HTML_DOCUMENT_NODE))
 	    {
 		xmlNsPtr *nsList, *curns, ns;
 
 		 
 
 		nsList = xmlGetNsList(node->doc, node);
 		if (nsList != NULL) {
 		    curns = nsList;
 		    do {
 			 
 			ns = xmlSearchNs(insert->doc, insert,
 			    (*curns)->prefix);
 
 			if ((ns == NULL) ||
 			    (! xmlStrEqual(ns->href, (*curns)->href)))
 			{
 			    ns = NULL;
 			     
 #if 0
 			    ns = xmlSearchNsByHref(insert->doc,
 				insert, (*curns)->href);
 #endif
 			}
 			if (ns == NULL) {
 			     
 			    ns = xmlNewNs(copy, (*curns)->href,
 				(*curns)->prefix);
 			     
 			}
 			if (node->ns == *curns) {
 			     
 			    copy->ns = ns;
 			}
 			curns++;
 		    } while (*curns != NULL);
 		    xmlFree(nsList);
 		}
 	    } else if (node->nsDef != NULL) {
 		 
 		if (node->nsDef != NULL) {
 		    if (isLRE)
 			xsltCopyNamespaceList(ctxt, copy, node->nsDef);
 		    else
 			xsltCopyNamespaceListInternal(copy, node->nsDef);
 		}
 	    }
 	     
 	    if (node->ns != NULL) {
 		if (copy->ns == NULL) {
 		     
 		    copy->ns = xsltGetSpecialNamespace(ctxt, invocNode,
 			node->ns->href, node->ns->prefix, copy);
 		}
 	    } else if ((insert->type == XML_ELEMENT_NODE) &&
 		(insert->ns != NULL))
 	    {
 		 
 		xsltGetSpecialNamespace(ctxt, invocNode, NULL, NULL, copy);
 	    }
 	     
 	    if (node->properties != NULL) {
 		xsltCopyAttrListNoOverwrite(ctxt, invocNode,
 		    copy, node->properties);
 	    }
 	    if (topElemVisited == 0)
 		topElemVisited = 1;
 	}
 	 
 	if (node->children != NULL) {
 	    xsltCopyTreeList(ctxt, invocNode,
 		node->children, copy, isLRE, topElemVisited);
 	}
     } else {
 	xsltTransformError(ctxt, NULL, invocNode,
 	    "xsltCopyTreeInternal: Copying of '%s' failed.\n", node->name);
     }
     return(copy);
 }
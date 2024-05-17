 xsltElement(xsltTransformContextPtr ctxt, xmlNodePtr node,
 	    xmlNodePtr inst, xsltStylePreCompPtr castedComp) {
 #ifdef XSLT_REFACTORED
     xsltStyleItemElementPtr comp = (xsltStyleItemElementPtr) castedComp;
 #else
     xsltStylePreCompPtr comp = castedComp;
 #endif
     xmlChar *prop = NULL;
     const xmlChar *name, *prefix = NULL, *nsName = NULL;
     xmlNodePtr copy;
     xmlNodePtr oldInsert;
 
     if (ctxt->insert == NULL)
 	return;
 
      
     if (!comp->has_name)
         return;
 
      
     oldInsert = ctxt->insert;
 
     if (comp->name == NULL) {
 	 
         prop = xsltEvalAttrValueTemplate(ctxt, inst,
 	    (const xmlChar *) "name", XSLT_NAMESPACE);
         if (prop == NULL) {
             xsltTransformError(ctxt, NULL, inst,
 		"xsl:element: The attribute 'name' is missing.\n");
             goto error;
         }
 	if (xmlValidateQName(prop, 0)) {
 	    xsltTransformError(ctxt, NULL, inst,
 		"xsl:element: The effective name '%s' is not a "
 		"valid QName.\n", prop);
 	     
 	}
 	name = xsltSplitQName(ctxt->dict, prop, &prefix);
 	xmlFree(prop);
     } else {
 	 
 #ifdef XSLT_REFACTORED
 	prefix = comp->nsPrefix;
 	name = comp->name;
 #else
 	name = xsltSplitQName(ctxt->dict, comp->name, &prefix);
 #endif
     }
 
      
     if (ctxt->output->dict == ctxt->dict) {
 	copy = xmlNewDocNodeEatName(ctxt->output, NULL, (xmlChar *)name, NULL);
     } else {
 	copy = xmlNewDocNode(ctxt->output, NULL, (xmlChar *)name, NULL);
     }
     if (copy == NULL) {
 	xsltTransformError(ctxt, NULL, inst,
 	    "xsl:element : creation of %s failed\n", name);
  	return;
      }
      copy = xsltAddChild(ctxt->insert, copy);
//     if (copy == NULL) {
//         xsltTransformError(ctxt, NULL, inst,
//             "xsl:element : xsltAddChild failed\n");
//         return;
//     }
  
       
     if (comp->has_ns) {
 	if (comp->ns != NULL) {
 	     
 	    if (comp->ns[0] != 0)
 		nsName = comp->ns;
 	} else {
 	    xmlChar *tmpNsName;
 	     
 	     
 	    tmpNsName = xsltEvalAttrValueTemplate(ctxt, inst,
 		(const xmlChar *) "namespace", XSLT_NAMESPACE);
 	     
 	    if ((tmpNsName != NULL) && (tmpNsName[0] != 0))
 		nsName = xmlDictLookup(ctxt->dict, BAD_CAST tmpNsName, -1);
 	    xmlFree(tmpNsName);
 	}
 
         if (xmlStrEqual(nsName, BAD_CAST "http://www.w3.org/2000/xmlns/")) {
             xsltTransformError(ctxt, NULL, inst,
                 "xsl:attribute: Namespace http://www.w3.org/2000/xmlns/ "
                 "forbidden.\n");
             goto error;
         }
         if (xmlStrEqual(nsName, XML_XML_NAMESPACE)) {
             prefix = BAD_CAST "xml";
         } else if (xmlStrEqual(prefix, BAD_CAST "xml")) {
             prefix = NULL;
         }
     } else {
 	xmlNsPtr ns;
 	 
 	ns = xmlSearchNs(inst->doc, inst, prefix);
 	if (ns == NULL) {
 	     
             if (prefix != NULL) {
                 xsltTransformError(ctxt, NULL, inst,
                     "xsl:element: The QName '%s:%s' has no "
                     "namespace binding in scope in the stylesheet; "
                     "this is an error, since the namespace was not "
                     "specified by the instruction itself.\n", prefix, name);
             }
 	} else
 	    nsName = ns->href;
     }
      
     if (nsName != NULL) {
 	if (xmlStrEqual(prefix, BAD_CAST "xmlns")) {
              
 	    xmlChar *pref = xmlStrdup(BAD_CAST "ns_1");
 
 	    copy->ns = xsltGetSpecialNamespace(ctxt, inst, nsName, pref, copy);
 
 	    xmlFree(pref);
 	} else {
 	    copy->ns = xsltGetSpecialNamespace(ctxt, inst, nsName, prefix,
 		copy);
 	}
     } else if ((copy->parent != NULL) &&
 	(copy->parent->type == XML_ELEMENT_NODE) &&
 	(copy->parent->ns != NULL))
     {
 	 
 	xsltGetSpecialNamespace(ctxt, inst, NULL, NULL, copy);
     }
 
     ctxt->insert = copy;
 
     if (comp->has_use) {
 	if (comp->use != NULL) {
 	    xsltApplyAttributeSet(ctxt, node, inst, comp->use);
 	} else {
 	    xmlChar *attrSets = NULL;
 	     
 	    attrSets = xsltEvalAttrValueTemplate(ctxt, inst,
 		(const xmlChar *)"use-attribute-sets", NULL);
 	    if (attrSets != NULL) {
 		xsltApplyAttributeSet(ctxt, node, inst, attrSets);
 		xmlFree(attrSets);
 	    }
 	}
     }
      
     if (inst->children != NULL)
 	xsltApplySequenceConstructor(ctxt, ctxt->node, inst->children,
 	    NULL);
 
 error:
     ctxt->insert = oldInsert;
     return;
 }
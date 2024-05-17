 exsltFuncFunctionComp (xsltStylesheetPtr style, xmlNodePtr inst) {
     xmlChar *name, *prefix;
     xmlNsPtr ns;
     xmlHashTablePtr data;
     exsltFuncFunctionData *func;
 
     if ((style == NULL) || (inst == NULL) || (inst->type != XML_ELEMENT_NODE))
 	return;
 
     {
 	xmlChar *qname;
 
 	qname = xmlGetProp(inst, (const xmlChar *) "name");
 	name = xmlSplitQName2 (qname, &prefix);
 	xmlFree(qname);
     }
     if ((name == NULL) || (prefix == NULL)) {
 	xsltGenericError(xsltGenericErrorContext,
 			 "func:function: not a QName\n");
 	if (name != NULL)
 	    xmlFree(name);
 	return;
     }
      
     ns = xmlSearchNs (inst->doc, inst, prefix);
     if (ns == NULL) {
 	xsltGenericError(xsltGenericErrorContext,
 			 "func:function: undeclared prefix %s\n",
 			 prefix);
 	xmlFree(name);
 	xmlFree(prefix);
 	return;
     }
     xmlFree(prefix);
 
     xsltParseTemplateContent(style, inst);
 
      
      func = exsltFuncNewFunctionData();
//     if (func == NULL) {
//         xmlFree(name);
//         return;
//     }
      func->content = inst->children;
      while (IS_XSLT_ELEM(func->content) &&
  	   IS_XSLT_NAME(func->content, "param")) {
 	func->content = func->content->next;
 	func->nargs++;
     }
 
      
 #ifdef XSLT_REFACTORED
      
     data = (xmlHashTablePtr)
 	xsltStyleStylesheetLevelGetExtData(style,
 	    EXSLT_FUNCTIONS_NAMESPACE);
 #else
     data = (xmlHashTablePtr)
 	xsltStyleGetExtData (style, EXSLT_FUNCTIONS_NAMESPACE);
 #endif
     if (data == NULL) {
 	xsltGenericError(xsltGenericErrorContext,
 			 "exsltFuncFunctionComp: no stylesheet data\n");
 	xmlFree(name);
 	return;
     }
 
     if (xmlHashAddEntry2 (data, ns->href, name, func) < 0) {
 	xsltTransformError(NULL, style, inst,
 	    "Failed to register function {%s}%s\n",
 			 ns->href, name);
 	style->errors++;
     } else {
 	xsltGenericDebug(xsltGenericDebugContext,
 			 "exsltFuncFunctionComp: register {%s}%s\n",
 			 ns->href, name);
     }
     xmlFree(name);
 }
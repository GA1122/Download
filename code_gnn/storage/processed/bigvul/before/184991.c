 xmlParsePEReference(xmlParserCtxtPtr ctxt)
 {
     const xmlChar *name;
     xmlEntityPtr entity = NULL;
     xmlParserInputPtr input;
 
     if (RAW != '%')
         return;
     NEXT;
     name = xmlParseName(ctxt);
     if (name == NULL) {
 	xmlFatalErrMsg(ctxt, XML_ERR_NAME_REQUIRED,
 		       "xmlParsePEReference: no name\n");
 	return;
     }
     if (RAW != ';') {
 	xmlFatalErr(ctxt, XML_ERR_ENTITYREF_SEMICOL_MISSING, NULL);
         return;
     }
 
     NEXT;
 
      
     ctxt->nbentities++;
 
      
      if ((ctxt->sax != NULL) &&
  	(ctxt->sax->getParameterEntity != NULL))
	entity = ctxt->sax->getParameterEntity(ctxt->userData,
					       name);
// 	entity = ctxt->sax->getParameterEntity(ctxt->userData, name);
//     if (ctxt->instate == XML_PARSER_EOF)
// 	return;
      if (entity == NULL) {
  	 
 	if ((ctxt->standalone == 1) ||
 	    ((ctxt->hasExternalSubset == 0) &&
 	     (ctxt->hasPErefs == 0))) {
 	    xmlFatalErrMsgStr(ctxt, XML_ERR_UNDECLARED_ENTITY,
 			      "PEReference: %%%s; not found\n",
 			      name);
 	} else {
 	     
 	    xmlWarningMsg(ctxt, XML_WAR_UNDECLARED_ENTITY,
 			  "PEReference: %%%s; not found\n",
 			  name, NULL);
 	    ctxt->valid = 0;
 	}
     } else {
 	 
 	if ((entity->etype != XML_INTERNAL_PARAMETER_ENTITY) &&
 	    (entity->etype != XML_EXTERNAL_PARAMETER_ENTITY)) {
 	    xmlWarningMsg(ctxt, XML_WAR_UNDECLARED_ENTITY,
 		  "Internal: %%%s; is not a parameter entity\n",
 			  name, NULL);
 	} else if (ctxt->input->free != deallocblankswrapper) {
 	    input = xmlNewBlanksWrapperInputStream(ctxt, entity);
 	    if (xmlPushInput(ctxt, input) < 0)
 		return;
 	} else {
 	     
 	    input = xmlNewEntityInputStream(ctxt, entity);
 	    if (xmlPushInput(ctxt, input) < 0)
 		return;
 	    if ((entity->etype == XML_EXTERNAL_PARAMETER_ENTITY) &&
 		(CMP5(CUR_PTR, '<', '?', 'x', 'm', 'l')) &&
 		(IS_BLANK_CH(NXT(5)))) {
 		xmlParseTextDecl(ctxt);
 		if (ctxt->errNo ==
 		    XML_ERR_UNSUPPORTED_ENCODING) {
 		     
 		    ctxt->instate = XML_PARSER_EOF;
 		    return;
 		}
 	    }
 	}
     }
     ctxt->hasPErefs = 1;
 }
 exsltFuncRegisterImportFunc (exsltFuncFunctionData *data,
 			     exsltFuncImportRegData *ch,
 			     const xmlChar *URI, const xmlChar *name,
 			     ATTRIBUTE_UNUSED const xmlChar *ignored) {
     exsltFuncFunctionData *func=NULL;
 
     if ((data == NULL) || (ch == NULL) || (URI == NULL) || (name == NULL))
             return;
 
     if (ch->ctxt == NULL || ch->hash == NULL)
 	return;
 
      
      func = (exsltFuncFunctionData*)xmlHashLookup2(ch->hash, URI, name);
      if (func == NULL) {		 
  	func = exsltFuncNewFunctionData();
//         if (func == NULL)
//             return;
  	memcpy(func, data, sizeof(exsltFuncFunctionData));
  	if (xmlHashAddEntry2(ch->hash, URI, name, func) < 0) {
  	    xsltGenericError(xsltGenericErrorContext,
 		    "Failed to register function {%s}%s\n",
 		    URI, name);
 	} else {		 
 	    xsltGenericDebug(xsltGenericDebugContext,
 	            "exsltFuncRegisterImportFunc: register {%s}%s\n",
 		    URI, name);
 	    xsltRegisterExtFunction(ch->ctxt, name, URI,
 		    exsltFuncFunctionFunction);
 	}
     }
 }
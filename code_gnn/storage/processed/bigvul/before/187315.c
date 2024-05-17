 exsltFuncFunctionFunction (xmlXPathParserContextPtr ctxt, int nargs) {
     xmlXPathObjectPtr oldResult, ret;
     exsltFuncData *data;
     exsltFuncFunctionData *func;
     xmlNodePtr paramNode, oldInsert, fake;
     int oldBase;
     xsltStackElemPtr params = NULL, param;
     xsltTransformContextPtr tctxt = xsltXPathGetTransformContext(ctxt);
     int i, notSet;
     struct objChain {
 	struct objChain *next;
 	xmlXPathObjectPtr obj;
     };
     struct objChain	*savedObjChain = NULL, *savedObj;
 
      
     data = (exsltFuncData *) xsltGetExtData (tctxt,
 					     EXSLT_FUNCTIONS_NAMESPACE);
     oldResult = data->result;
     data->result = NULL;
 
      func = (exsltFuncFunctionData*) xmlHashLookup2 (data->funcs,
  						    ctxt->context->functionURI,
  						    ctxt->context->function);
//     if (func == NULL) {
//          
//         xsltGenericError(xsltGenericErrorContext,
//                          "{%s}%s: not found\n",
//                          ctxt->context->functionURI, ctxt->context->function);
//         ctxt->error = XPATH_UNKNOWN_FUNC_ERROR;
//         return;
//     }
  
       
     if (nargs > func->nargs) {
 	xsltGenericError(xsltGenericErrorContext,
 			 "{%s}%s: called with too many arguments\n",
 			 ctxt->context->functionURI, ctxt->context->function);
 	ctxt->error = XPATH_INVALID_ARITY;
 	return;
     }
     if (func->content != NULL) {
 	paramNode = func->content->prev;
     }
     else
 	paramNode = NULL;
     if ((paramNode == NULL) && (func->nargs != 0)) {
 	xsltGenericError(xsltGenericErrorContext,
 			 "exsltFuncFunctionFunction: nargs != 0 and "
 			 "param == NULL\n");
 	return;
     }
     if (tctxt->funcLevel > MAX_FUNC_RECURSION) {
 	xsltGenericError(xsltGenericErrorContext,
 			 "{%s}%s: detected a recursion\n",
 			 ctxt->context->functionURI, ctxt->context->function);
 	ctxt->error = XPATH_MEMORY_ERROR;
 	return;
     }
     tctxt->funcLevel++;
 
      
      
     oldBase = tctxt->varsBase;
     tctxt->varsBase = tctxt->varsNr;
      
     if (paramNode != NULL) {
          
 	for (i = 0; i < nargs; i++) {
 	    savedObj = xmlMalloc(sizeof(struct objChain));
 	    savedObj->next = savedObjChain;
 	    savedObj->obj = valuePop(ctxt);
 	    savedObjChain = savedObj;
 	}
 
 	 
 	for (i = 1; i <= func->nargs; i++) {
 	    if (paramNode->prev == NULL)
 	        break;
 	    paramNode = paramNode->prev;
 	}
 	 
 	notSet = func->nargs - nargs;
 	for (; i > 0; i--) {
 	    param = xsltParseStylesheetCallerParam (tctxt, paramNode);
 	    if (i > notSet) {	 
 		param->computed = 1;
 		if (param->value != NULL)
 		    xmlXPathFreeObject(param->value);
 		savedObj = savedObjChain;	 
 		param->value = savedObj->obj;
 		savedObjChain = savedObjChain->next;
 		xmlFree(savedObj);
 	    }
 	    xsltLocalVariablePush(tctxt, param, -1);
 	    param->next = params;
 	    params = param;
 	    paramNode = paramNode->next;
 	}
     }
      
     fake = xmlNewDocNode(tctxt->output, NULL,
 			 (const xmlChar *)"fake", NULL);
     oldInsert = tctxt->insert;
     tctxt->insert = fake;
     xsltApplyOneTemplate (tctxt, xmlXPathGetContextNode(ctxt),
 			  func->content, NULL, NULL);
     xsltLocalVariablePop(tctxt, tctxt->varsBase, -2);
     tctxt->insert = oldInsert;
     tctxt->varsBase = oldBase;	 
     if (params != NULL)
 	xsltFreeStackElemList(params);
 
     if (data->error != 0)
 	goto error;
 
     if (data->result != NULL) {
 	ret = data->result;
     } else
 	ret = xmlXPathNewCString("");
 
     data->result = oldResult;
 
      
     if (fake->children != NULL) {
 #ifdef LIBXML_DEBUG_ENABLED
 	xmlDebugDumpNode (stderr, fake, 1);
 #endif
 	xsltGenericError(xsltGenericErrorContext,
 			 "{%s}%s: cannot write to result tree while "
 			 "executing a function\n",
 			 ctxt->context->functionURI, ctxt->context->function);
 	xmlFreeNode(fake);
 	goto error;
     }
     xmlFreeNode(fake);
     valuePush(ctxt, ret);
 
 error:
      
     xsltExtensionInstructionResultFinalize(tctxt);
     tctxt->funcLevel--;
 }
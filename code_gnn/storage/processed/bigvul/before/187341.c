 xsltApplyStylesheetInternal(xsltStylesheetPtr style, xmlDocPtr doc,
                             const char **params, const char *output,
                             FILE * profile, xsltTransformContextPtr userCtxt)
 {
     xmlDocPtr res = NULL;
     xsltTransformContextPtr ctxt = NULL;
     xmlNodePtr root, node;
     const xmlChar *method;
     const xmlChar *doctypePublic;
     const xmlChar *doctypeSystem;
     const xmlChar *version;
     const xmlChar *encoding;
     xsltStackElemPtr variables;
     xsltStackElemPtr vptr;
 
     xsltInitGlobals();
 
     if ((style == NULL) || (doc == NULL))
         return (NULL);
 
     if (style->internalized == 0) {
 #ifdef WITH_XSLT_DEBUG
 	xsltGenericDebug(xsltGenericDebugContext,
 			 "Stylesheet was not fully internalized !\n");
 #endif
     }
     if (doc->intSubset != NULL) {
 	 
 	xmlNodePtr cur = (xmlNodePtr) doc->intSubset;
 	if (cur->next != NULL)
 	    cur->next->prev = cur->prev;
 	if (cur->prev != NULL)
 	    cur->prev->next = cur->next;
 	if (doc->children == cur)
 	    doc->children = cur->next;
 	if (doc->last == cur)
 	    doc->last = cur->prev;
 	cur->prev = cur->next = NULL;
     }
 
      
     root = xmlDocGetRootElement(doc);
     if (root != NULL) {
 	if (((long) root->content) >= 0 && (xslDebugStatus == XSLT_DEBUG_NONE))
 	    xmlXPathOrderDocElems(doc);
     }
 
     if (userCtxt != NULL)
 	ctxt = userCtxt;
     else
 	ctxt = xsltNewTransformContext(style, doc);
 
     if (ctxt == NULL)
         return (NULL);
 
     ctxt->initialContextDoc = doc;
     ctxt->initialContextNode = (xmlNodePtr) doc;
 
     if (profile != NULL)
         ctxt->profile = 1;
 
     if (output != NULL)
         ctxt->outputFile = output;
     else
         ctxt->outputFile = NULL;
 
      
     if (ctxt->dict != NULL) {
         if (ctxt->mode != NULL)
 	    ctxt->mode = xmlDictLookup(ctxt->dict, ctxt->mode, -1);
         if (ctxt->modeURI != NULL)
 	    ctxt->modeURI = xmlDictLookup(ctxt->dict, ctxt->modeURI, -1);
     }
 
     XSLT_GET_IMPORT_PTR(method, style, method)
     XSLT_GET_IMPORT_PTR(doctypePublic, style, doctypePublic)
     XSLT_GET_IMPORT_PTR(doctypeSystem, style, doctypeSystem)
     XSLT_GET_IMPORT_PTR(version, style, version)
     XSLT_GET_IMPORT_PTR(encoding, style, encoding)
 
     if ((method != NULL) &&
 	(!xmlStrEqual(method, (const xmlChar *) "xml")))
     {
         if (xmlStrEqual(method, (const xmlChar *) "html")) {
             ctxt->type = XSLT_OUTPUT_HTML;
             if (((doctypePublic != NULL) || (doctypeSystem != NULL))) {
                 res = htmlNewDoc(doctypeSystem, doctypePublic);
 	    } else {
                 if (version == NULL) {
 		    xmlDtdPtr dtd;
 
 		    res = htmlNewDoc(NULL, NULL);
 		     
 		    if (res != NULL) {
 			dtd = xmlGetIntSubset(res);
 			if (dtd != NULL) {
 			    xmlUnlinkNode((xmlNodePtr) dtd);
 			    xmlFreeDtd(dtd);
 			}
 			res->intSubset = NULL;
 			res->extSubset = NULL;
 		    }
 		} else {
 
 #ifdef XSLT_GENERATE_HTML_DOCTYPE
 		    xsltGetHTMLIDs(version, &doctypePublic, &doctypeSystem);
 #endif
 		    res = htmlNewDoc(doctypeSystem, doctypePublic);
 		}
             }
             if (res == NULL)
                 goto error;
 	    res->dict = ctxt->dict;
 	    xmlDictReference(res->dict);
 
 #ifdef WITH_XSLT_DEBUG
 	    xsltGenericDebug(xsltGenericDebugContext,
 		"reusing transformation dict for output\n");
  #endif
          } else if (xmlStrEqual(method, (const xmlChar *) "xhtml")) {
  	    xsltTransformError(ctxt, NULL, (xmlNodePtr) doc,
		"xsltApplyStylesheetInternal: unsupported method xhtml, using html\n",
		style->method);
// 		"xsltApplyStylesheetInternal: unsupported method xhtml, using html\n");
              ctxt->type = XSLT_OUTPUT_HTML;
              res = htmlNewDoc(doctypeSystem, doctypePublic);
              if (res == NULL)
                 goto error;
 	    res->dict = ctxt->dict;
 	    xmlDictReference(res->dict);
 
 #ifdef WITH_XSLT_DEBUG
 	    xsltGenericDebug(xsltGenericDebugContext,
 		"reusing transformation dict for output\n");
 #endif
         } else if (xmlStrEqual(method, (const xmlChar *) "text")) {
             ctxt->type = XSLT_OUTPUT_TEXT;
             res = xmlNewDoc(style->version);
             if (res == NULL)
                 goto error;
 	    res->dict = ctxt->dict;
 	    xmlDictReference(res->dict);
 
 #ifdef WITH_XSLT_DEBUG
 	    xsltGenericDebug(xsltGenericDebugContext,
 		"reusing transformation dict for output\n");
  #endif
          } else {
  	    xsltTransformError(ctxt, NULL, (xmlNodePtr) doc,
		"xsltApplyStylesheetInternal: unsupported method %s\n",
		style->method);
// 		"xsltApplyStylesheetInternal: unsupported method (%s)\n",
// 		method);
              goto error;
          }
      } else {
         ctxt->type = XSLT_OUTPUT_XML;
         res = xmlNewDoc(style->version);
         if (res == NULL)
             goto error;
 	res->dict = ctxt->dict;
 	xmlDictReference(ctxt->dict);
 #ifdef WITH_XSLT_DEBUG
 	xsltGenericDebug(xsltGenericDebugContext,
 			 "reusing transformation dict for output\n");
 #endif
     }
     res->charset = XML_CHAR_ENCODING_UTF8;
     if (encoding != NULL)
         res->encoding = xmlStrdup(encoding);
     variables = style->variables;
 
      
     if (xsltNeedElemSpaceHandling(ctxt))
 	xsltApplyStripSpaces(ctxt, xmlDocGetRootElement(doc));
      
     ctxt->node = (xmlNodePtr) doc;
     if (ctxt->globalVars == NULL)
 	ctxt->globalVars = xmlHashCreate(20);
     if (params != NULL) {
         xsltEvalUserParams(ctxt, params);
     }
 
      
     xsltCountKeys(ctxt);
 
     xsltEvalGlobalVariables(ctxt);
 
     ctxt->node = (xmlNodePtr) doc;
     ctxt->output = res;
     ctxt->insert = (xmlNodePtr) res;
     ctxt->varsBase = ctxt->varsNr - 1;
 
     ctxt->xpathCtxt->contextSize = 1;
     ctxt->xpathCtxt->proximityPosition = 1;
     ctxt->xpathCtxt->node = NULL;  
      
     xsltProcessOneNode(ctxt, ctxt->node, NULL);
      
     xsltLocalVariablePop(ctxt, 0, -2);
     xsltShutdownCtxtExts(ctxt);
 
     xsltCleanupTemplates(style);  
 
      
     if (style->variables != variables) {
         vptr = style->variables;
         while (vptr->next != variables)
             vptr = vptr->next;
         vptr->next = NULL;
         xsltFreeStackElemList(style->variables);
         style->variables = variables;
     }
     vptr = style->variables;
     while (vptr != NULL) {
         if (vptr->computed) {
             if (vptr->value != NULL) {
                 xmlXPathFreeObject(vptr->value);
                 vptr->value = NULL;
                 vptr->computed = 0;
             }
         }
         vptr = vptr->next;
     }
 #if 0
      
      
     xsltFreeRVTs(ctxt);
 #endif
      
     root = xmlDocGetRootElement(res);
     if (root != NULL) {
         const xmlChar *doctype = NULL;
 
         if ((root->ns != NULL) && (root->ns->prefix != NULL))
 	    doctype = xmlDictQLookup(ctxt->dict, root->ns->prefix, root->name);
 	if (doctype == NULL)
 	    doctype = root->name;
 
          
         if ((method == NULL) &&
             (root->ns == NULL) &&
             (!xmlStrcasecmp(root->name, (const xmlChar *) "html"))) {
             xmlNodePtr tmp;
 
             tmp = res->children;
             while ((tmp != NULL) && (tmp != root)) {
                 if (tmp->type == XML_ELEMENT_NODE)
                     break;
                 if ((tmp->type == XML_TEXT_NODE) && (!xmlIsBlankNode(tmp)))
                     break;
 		tmp = tmp->next;
             }
             if (tmp == root) {
                 ctxt->type = XSLT_OUTPUT_HTML;
 		 
                 res->type = XML_HTML_DOCUMENT_NODE;
                 if (((doctypePublic != NULL) || (doctypeSystem != NULL))) {
                     res->intSubset = xmlCreateIntSubset(res, doctype,
                                                         doctypePublic,
                                                         doctypeSystem);
 #ifdef XSLT_GENERATE_HTML_DOCTYPE
 		} else if (version != NULL) {
                     xsltGetHTMLIDs(version, &doctypePublic,
                                    &doctypeSystem);
                     if (((doctypePublic != NULL) || (doctypeSystem != NULL)))
                         res->intSubset =
                             xmlCreateIntSubset(res, doctype,
                                                doctypePublic,
                                                doctypeSystem);
 #endif
                 }
             }
 
         }
         if (ctxt->type == XSLT_OUTPUT_XML) {
             XSLT_GET_IMPORT_PTR(doctypePublic, style, doctypePublic)
             XSLT_GET_IMPORT_PTR(doctypeSystem, style, doctypeSystem)
             if (((doctypePublic != NULL) || (doctypeSystem != NULL))) {
 	        xmlNodePtr last;
 		 
 		node = res->children;
 		last = res->last;
 		res->children = NULL;
 		res->last = NULL;
                 res->intSubset = xmlCreateIntSubset(res, doctype,
                                                     doctypePublic,
                                                     doctypeSystem);
 		if (res->children != NULL) {
 		    res->children->next = node;
 		    node->prev = res->children;
 		    res->last = last;
 		} else {
 		    res->children = node;
 		    res->last = last;
 		}
 	    }
         }
     }
     xmlXPathFreeNodeSet(ctxt->nodeList);
     if (profile != NULL) {
         xsltSaveProfiling(ctxt, profile);
     }
 
      
     if ((ctxt != NULL) && (ctxt->state == XSLT_STATE_ERROR)) {
 	xmlFreeDoc(res);
 	res = NULL;
     }
     if ((res != NULL) && (ctxt != NULL) && (output != NULL)) {
 	int ret;
 
 	ret = xsltCheckWrite(ctxt->sec, ctxt, (const xmlChar *) output);
 	if (ret == 0) {
 	    xsltTransformError(ctxt, NULL, NULL,
 		     "xsltApplyStylesheet: forbidden to save to %s\n",
 			       output);
 	} else if (ret < 0) {
 	    xsltTransformError(ctxt, NULL, NULL,
 		     "xsltApplyStylesheet: saving to %s may not be possible\n",
 			       output);
 	}
     }
 
 #ifdef XSLT_DEBUG_PROFILE_CACHE
     printf("# Cache:\n");
     printf("# Reused tree fragments: %d\n", ctxt->cache->dbgReusedRVTs);
     printf("# Reused variables     : %d\n", ctxt->cache->dbgReusedVars);
 #endif
 
     if ((ctxt != NULL) && (userCtxt == NULL))
 	xsltFreeTransformContext(ctxt);
 
     return (res);
 
 error:
     if (res != NULL)
         xmlFreeDoc(res);
 
 #ifdef XSLT_DEBUG_PROFILE_CACHE
     printf("# Cache:\n");
     printf("# Reused tree fragments: %d\n", ctxt->cache->dbgReusedRVTs);
     printf("# Reused variables     : %d\n", ctxt->cache->dbgReusedVars);
 #endif
 
     if ((ctxt != NULL) && (userCtxt == NULL))
         xsltFreeTransformContext(ctxt);
     return (NULL);
 }
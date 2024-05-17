  xsltTestCompMatch(xsltTransformContextPtr ctxt, xsltCompMatchPtr comp,
	          xmlNodePtr node, const xmlChar *mode,
// 	          xmlNodePtr matchNode, const xmlChar *mode,
  		  const xmlChar *modeURI) {
      int i;
//     xmlNodePtr node = matchNode;
      xsltStepOpPtr step, sel = NULL;
      xsltStepStates states = {0, 0, NULL};  
  
     if ((comp == NULL) || (node == NULL) || (ctxt == NULL)) {
 	xsltTransformError(ctxt, NULL, node,
 		"xsltTestCompMatch: null arg\n");
         return(-1);
     }
     if (mode != NULL) {
 	if (comp->mode == NULL)
 	    return(0);
 	 
 	if (comp->mode != mode)
 	    return(0);
     } else {
 	if (comp->mode != NULL)
 	    return(0);
     }
     if (modeURI != NULL) {
 	if (comp->modeURI == NULL)
 	    return(0);
 	 
 	if (comp->modeURI != modeURI)
 	    return(0);
     } else {
 	if (comp->modeURI != NULL)
 	    return(0);
     }
 
     i = 0;
 restart:
     for (;i < comp->nbStep;i++) {
 	step = &comp->steps[i];
 	if (step->op != XSLT_OP_PREDICATE)
 	    sel = step;
 	switch (step->op) {
             case XSLT_OP_END:
 		goto found;
             case XSLT_OP_ROOT:
 		if ((node->type == XML_DOCUMENT_NODE) ||
 #ifdef LIBXML_DOCB_ENABLED
 		    (node->type == XML_DOCB_DOCUMENT_NODE) ||
 #endif
 		    (node->type == XML_HTML_DOCUMENT_NODE))
 		    continue;
 		if ((node->type == XML_ELEMENT_NODE) && (node->name[0] == ' '))
 		    continue;
 		goto rollback;
             case XSLT_OP_ELEM:
 		if (node->type != XML_ELEMENT_NODE)
 		    goto rollback;
 		if (step->value == NULL)
 		    continue;
 		if (step->value[0] != node->name[0])
 		    goto rollback;
 		if (!xmlStrEqual(step->value, node->name))
 		    goto rollback;
 
 		 
 		if (node->ns == NULL) {
 		    if (step->value2 != NULL)
 			goto rollback;
 		} else if (node->ns->href != NULL) {
 		    if (step->value2 == NULL)
 			goto rollback;
 		    if (!xmlStrEqual(step->value2, node->ns->href))
 			goto rollback;
 		}
 		continue;
             case XSLT_OP_ATTR:
 		if (node->type != XML_ATTRIBUTE_NODE)
 		    goto rollback;
 		if (step->value != NULL) {
 		    if (step->value[0] != node->name[0])
 			goto rollback;
 		    if (!xmlStrEqual(step->value, node->name))
 			goto rollback;
 		}
 		 
 		if (node->ns == NULL) {
 		    if (step->value2 != NULL)
 			goto rollback;
 		} else if (step->value2 != NULL) {
 		    if (!xmlStrEqual(step->value2, node->ns->href))
 			goto rollback;
 		}
 		continue;
             case XSLT_OP_PARENT:
 		if ((node->type == XML_DOCUMENT_NODE) ||
 		    (node->type == XML_HTML_DOCUMENT_NODE) ||
 #ifdef LIBXML_DOCB_ENABLED
 		    (node->type == XML_DOCB_DOCUMENT_NODE) ||
 #endif
 		    (node->type == XML_NAMESPACE_DECL))
 		    goto rollback;
 		node = node->parent;
 		if (node == NULL)
 		    goto rollback;
 		if (step->value == NULL)
 		    continue;
 		if (step->value[0] != node->name[0])
 		    goto rollback;
 		if (!xmlStrEqual(step->value, node->name))
 		    goto rollback;
 		 
 		if (node->ns == NULL) {
 		    if (step->value2 != NULL)
 			goto rollback;
 		} else if (node->ns->href != NULL) {
 		    if (step->value2 == NULL)
 			goto rollback;
 		    if (!xmlStrEqual(step->value2, node->ns->href))
 			goto rollback;
 		}
 		continue;
             case XSLT_OP_ANCESTOR:
 		 
 		if (step->value == NULL) {
 		    step = &comp->steps[i+1];
 		    if (step->op == XSLT_OP_ROOT)
 			goto found;
 		     
 		    if ((step->op != XSLT_OP_ELEM) &&
 			(step->op != XSLT_OP_ALL) &&
 			(step->op != XSLT_OP_NS) &&
 			(step->op != XSLT_OP_ID) &&
 			(step->op != XSLT_OP_KEY))
 			goto rollback;
 		}
 		if (node == NULL)
 		    goto rollback;
 		if ((node->type == XML_DOCUMENT_NODE) ||
 		    (node->type == XML_HTML_DOCUMENT_NODE) ||
 #ifdef LIBXML_DOCB_ENABLED
 		    (node->type == XML_DOCB_DOCUMENT_NODE) ||
 #endif
 		    (node->type == XML_NAMESPACE_DECL))
 		    goto rollback;
 		node = node->parent;
 		if ((step->op != XSLT_OP_ELEM) && step->op != XSLT_OP_ALL) {
 		    xsltPatPushState(ctxt, &states, i, node);
 		    continue;
 		}
 		i++;
 		if (step->value == NULL) {
 		    xsltPatPushState(ctxt, &states, i - 1, node);
 		    continue;
 		}
 		while (node != NULL) {
 		    if ((node->type == XML_ELEMENT_NODE) &&
 			(step->value[0] == node->name[0]) &&
 			(xmlStrEqual(step->value, node->name))) {
 			 
 			if (node->ns == NULL) {
 			    if (step->value2 == NULL)
 				break;
 			} else if (node->ns->href != NULL) {
 			    if ((step->value2 != NULL) &&
 			        (xmlStrEqual(step->value2, node->ns->href)))
 				break;
 			}
 		    }
 		    node = node->parent;
 		}
 		if (node == NULL)
 		    goto rollback;
 		xsltPatPushState(ctxt, &states, i - 1, node);
 		continue;
             case XSLT_OP_ID: {
 		 
 		xmlAttrPtr id;
 
 		if (node->type != XML_ELEMENT_NODE)
 		    goto rollback;
 
 		id = xmlGetID(node->doc, step->value);
 		if ((id == NULL) || (id->parent != node))
 		    goto rollback;
 		break;
 	    }
             case XSLT_OP_KEY: {
 		xmlNodeSetPtr list;
 		int indx;
 
 		list = xsltGetKey(ctxt, step->value,
 			          step->value3, step->value2);
 		if (list == NULL)
 		    goto rollback;
 		for (indx = 0;indx < list->nodeNr;indx++)
 		    if (list->nodeTab[indx] == node)
 			break;
 		if (indx >= list->nodeNr)
 		    goto rollback;
 		break;
 	    }
             case XSLT_OP_NS:
 		if (node->type != XML_ELEMENT_NODE)
 		    goto rollback;
 		if (node->ns == NULL) {
 		    if (step->value != NULL)
 			goto rollback;
 		} else if (node->ns->href != NULL) {
 		    if (step->value == NULL)
 			goto rollback;
 		    if (!xmlStrEqual(step->value, node->ns->href))
 			goto rollback;
 		}
 		break;
             case XSLT_OP_ALL:
 		if (node->type != XML_ELEMENT_NODE)
  		    goto rollback;
  		break;
  	    case XSLT_OP_PREDICATE: {
		xmlNodePtr oldNode;
		xmlDocPtr doc;
		int oldCS, oldCP;
		int pos = 0, len = 0;
		int isRVT;
  		 
 		if (comp->direct) {
 		    if (states.states != NULL) {
  			 
  			xmlFree(states.states);
  		    }
		    return(xsltTestCompMatchDirect(ctxt, comp, node,
// 		    return(xsltTestCompMatchDirect(ctxt, comp, matchNode,
  						   comp->nsList, comp->nsNr));
  		}
  
		doc = node->doc;
		if (XSLT_IS_RES_TREE_FRAG(doc))
		    isRVT = 1;
		else
		    isRVT = 0;
		 
		oldCS = ctxt->xpathCtxt->contextSize;
		oldCP = ctxt->xpathCtxt->proximityPosition;
		if ((sel != NULL) &&
		    (sel->op == XSLT_OP_ELEM) &&
		    (sel->value != NULL) &&
		    (node->type == XML_ELEMENT_NODE) &&
		    (node->parent != NULL)) {
		    xmlNodePtr previous;
		    int nocache = 0;
		    previous = (xmlNodePtr)
			XSLT_RUNTIME_EXTRA(ctxt, sel->previousExtra, ptr);
		    if ((previous != NULL) &&
			(previous->parent == node->parent)) {
			 
			int indx = 0;
			xmlNodePtr sibling = node;
			while (sibling != NULL) {
			    if (sibling == previous)
				break;
			    if ((sibling->type == XML_ELEMENT_NODE) &&
				(previous->name != NULL) &&
				(sibling->name != NULL) &&
				(previous->name[0] == sibling->name[0]) &&
				(xmlStrEqual(previous->name, sibling->name)))
			    {
				if ((sel->value2 == NULL) ||
				    ((sibling->ns != NULL) &&
				     (xmlStrEqual(sel->value2,
						  sibling->ns->href))))
				    indx++;
			    }
			    sibling = sibling->prev;
			}
			if (sibling == NULL) {
			     
			    indx = 0;
			    sibling = node;
			    while (sibling != NULL) {
				if (sibling == previous)
				    break;
				if ((sibling->type == XML_ELEMENT_NODE) &&
				    (previous->name != NULL) &&
				    (sibling->name != NULL) &&
				    (previous->name[0] == sibling->name[0]) &&
				    (xmlStrEqual(previous->name, sibling->name)))
				{
				    if ((sel->value2 == NULL) ||
					((sibling->ns != NULL) &&
					(xmlStrEqual(sel->value2,
					sibling->ns->href))))
				    {
					indx--;
				    }
				}
				sibling = sibling->next;
			    }
			}
			if (sibling != NULL) {
		            pos = XSLT_RUNTIME_EXTRA(ctxt,
                                sel->indexExtra, ival) + indx;
			     
			    if (node->doc != NULL) {
				len = XSLT_RUNTIME_EXTRA(ctxt,
				        sel->lenExtra, ival);
				if (!isRVT) {
				    XSLT_RUNTIME_EXTRA(ctxt,
					sel->previousExtra, ptr) = node;
				    XSLT_RUNTIME_EXTRA(ctxt,
				        sel->indexExtra, ival) = pos;
				}
			    }
			} else
			    pos = 0;
		    } else {
			 
			xmlNodePtr parent = node->parent;
			xmlNodePtr siblings = NULL;
                        if (parent) siblings = parent->children;
			while (siblings != NULL) {
			    if (siblings->type == XML_ELEMENT_NODE) {
				if (siblings == node) {
				    len++;
				    pos = len;
				} else if ((node->name != NULL) &&
					   (siblings->name != NULL) &&
				    (node->name[0] == siblings->name[0]) &&
				    (xmlStrEqual(node->name, siblings->name))) {
				    if ((sel->value2 == NULL) ||
					((siblings->ns != NULL) &&
					 (xmlStrEqual(sel->value2,
						      siblings->ns->href))))
					len++;
				}
			    }
			    siblings = siblings->next;
			}
			if ((parent == NULL) || (node->doc == NULL))
			    nocache = 1;
			else {
			    while (parent->parent != NULL)
				parent = parent->parent;
			    if (((parent->type != XML_DOCUMENT_NODE) &&
				 (parent->type != XML_HTML_DOCUMENT_NODE)) ||
				 (parent != (xmlNodePtr) node->doc))
				nocache = 1;
			}
		    }
		    if (pos != 0) {
			ctxt->xpathCtxt->contextSize = len;
			ctxt->xpathCtxt->proximityPosition = pos;
			 
			if ((!isRVT) && (node->doc != NULL) &&
			    (nocache == 0)) {
			    XSLT_RUNTIME_EXTRA(ctxt, sel->previousExtra, ptr) =
				node;
			    XSLT_RUNTIME_EXTRA(ctxt, sel->indexExtra, ival) =
				pos;
			    XSLT_RUNTIME_EXTRA(ctxt, sel->lenExtra, ival) =
				len;
			}
		    }
		} else if ((sel != NULL) && (sel->op == XSLT_OP_ALL) &&
			   (node->type == XML_ELEMENT_NODE)) {
		    xmlNodePtr previous;
		    int nocache = 0;
		    previous = (xmlNodePtr)
			XSLT_RUNTIME_EXTRA(ctxt, sel->previousExtra, ptr);
		    if ((previous != NULL) &&
			(previous->parent == node->parent)) {
			 
			int indx = 0;
			xmlNodePtr sibling = node;
			while (sibling != NULL) {
			    if (sibling == previous)
				break;
			    if (sibling->type == XML_ELEMENT_NODE)
				indx++;
			    sibling = sibling->prev;
			}
			if (sibling == NULL) {
			     
			    indx = 0;
			    sibling = node;
			    while (sibling != NULL) {
				if (sibling == previous)
				    break;
				if (sibling->type == XML_ELEMENT_NODE)
				    indx--;
				sibling = sibling->next;
			    }
			}
			if (sibling != NULL) {
			    pos = XSLT_RUNTIME_EXTRA(ctxt,
                                sel->indexExtra, ival) + indx;
			     
			    if ((node->doc != NULL) && !isRVT) {
				len = XSLT_RUNTIME_EXTRA(ctxt,
				        sel->lenExtra, ival);
				XSLT_RUNTIME_EXTRA(ctxt,
					sel->previousExtra, ptr) = node;
				XSLT_RUNTIME_EXTRA(ctxt,
					sel->indexExtra, ival) = pos;
			    }
			} else
			    pos = 0;
		    } else {
			 
			xmlNodePtr parent = node->parent;
			xmlNodePtr siblings = NULL;
                        if (parent) siblings = parent->children;
			while (siblings != NULL) {
			    if (siblings->type == XML_ELEMENT_NODE) {
				len++;
				if (siblings == node) {
				    pos = len;
				}
			    }
			    siblings = siblings->next;
			}
			if ((parent == NULL) || (node->doc == NULL))
			    nocache = 1;
			else {
			    while (parent->parent != NULL)
				parent = parent->parent;
			    if (((parent->type != XML_DOCUMENT_NODE) &&
				 (parent->type != XML_HTML_DOCUMENT_NODE)) ||
				 (parent != (xmlNodePtr) node->doc))
				nocache = 1;
			}
		    }
		    if (pos != 0) {
			ctxt->xpathCtxt->contextSize = len;
			ctxt->xpathCtxt->proximityPosition = pos;
			 
			if ((node->doc != NULL) && (nocache == 0) && !isRVT) {
			    XSLT_RUNTIME_EXTRA(ctxt, sel->previousExtra, ptr) =
				node;
			    XSLT_RUNTIME_EXTRA(ctxt, sel->indexExtra, ival) =
				pos;
			    XSLT_RUNTIME_EXTRA(ctxt, sel->lenExtra, ival) =
				len;
			}
		    }
		}
		oldNode = ctxt->node;
		ctxt->node = node;
		if (step->value == NULL)
		    goto wrong_index;
		if (step->comp == NULL)
		    goto wrong_index;
		if (!xsltEvalXPathPredicate(ctxt, step->comp, comp->nsList,
			                    comp->nsNr))
		    goto wrong_index;
// 		if (!xsltTestPredicateMatch(ctxt, comp, node, step, sel))
// 		    goto rollback;
  
		if (pos != 0) {
		    ctxt->xpathCtxt->contextSize = oldCS;
		    ctxt->xpathCtxt->proximityPosition = oldCP;
		}
		ctxt->node = oldNode;
  		break;
wrong_index:
		if (pos != 0) {
		    ctxt->xpathCtxt->contextSize = oldCS;
		    ctxt->xpathCtxt->proximityPosition = oldCP;
		}
		ctxt->node = oldNode;
		goto rollback;
  	    }
              case XSLT_OP_PI:
  		if (node->type != XML_PI_NODE)
 		    goto rollback;
 		if (step->value != NULL) {
 		    if (!xmlStrEqual(step->value, node->name))
 			goto rollback;
 		}
 		break;
             case XSLT_OP_COMMENT:
 		if (node->type != XML_COMMENT_NODE)
 		    goto rollback;
 		break;
             case XSLT_OP_TEXT:
 		if ((node->type != XML_TEXT_NODE) &&
 		    (node->type != XML_CDATA_SECTION_NODE))
 		    goto rollback;
 		break;
             case XSLT_OP_NODE:
 		switch (node->type) {
 		    case XML_ELEMENT_NODE:
 		    case XML_CDATA_SECTION_NODE:
 		    case XML_PI_NODE:
 		    case XML_COMMENT_NODE:
 		    case XML_TEXT_NODE:
 			break;
 		    default:
 			goto rollback;
 		}
 		break;
 	}
     }
 found:
     if (states.states != NULL) {
          
 	xmlFree(states.states);
     }
     return(1);
 rollback:
      
     if (states.states == NULL)
 	return(0);
     if (states.nbstates <= 0) {
 	xmlFree(states.states);
 	return(0);
     }
     states.nbstates--;
     i = states.states[states.nbstates].step;
     node = states.states[states.nbstates].node;
 #if 0
     fprintf(stderr, "Pop: %d, %s\n", i, node->name);
 #endif
     goto restart;
 }
xmlParseReference(xmlParserCtxtPtr ctxt) {
    xmlEntityPtr ent;
    xmlChar *val;
    int was_checked;
    xmlNodePtr list = NULL;
    xmlParserErrors ret = XML_ERR_OK;


    if (RAW != '&')
        return;

     
    if (NXT(1) == '#') {
	int i = 0;
	xmlChar out[10];
	int hex = NXT(2);
	int value = xmlParseCharRef(ctxt);

	if (value == 0)
	    return;
	if (ctxt->charset != XML_CHAR_ENCODING_UTF8) {
	     
	    if (value <= 0xFF) {
		out[0] = value;
		out[1] = 0;
		if ((ctxt->sax != NULL) && (ctxt->sax->characters != NULL) &&
		    (!ctxt->disableSAX))
		    ctxt->sax->characters(ctxt->userData, out, 1);
	    } else {
		if ((hex == 'x') || (hex == 'X'))
		    snprintf((char *)out, sizeof(out), "#x%X", value);
		else
		    snprintf((char *)out, sizeof(out), "#%d", value);
		if ((ctxt->sax != NULL) && (ctxt->sax->reference != NULL) &&
		    (!ctxt->disableSAX))
		    ctxt->sax->reference(ctxt->userData, out);
	    }
	} else {
	     
	    COPY_BUF(0 ,out, i, value);
	    out[i] = 0;
	    if ((ctxt->sax != NULL) && (ctxt->sax->characters != NULL) &&
		(!ctxt->disableSAX))
		ctxt->sax->characters(ctxt->userData, out, i);
	}
	return;
    }

     
    ent = xmlParseEntityRef(ctxt);
    if (ent == NULL) return;
    if (!ctxt->wellFormed)
	return;
    was_checked = ent->checked;

     
    if ((ent->name == NULL) ||
        (ent->etype == XML_INTERNAL_PREDEFINED_ENTITY)) {
	val = ent->content;
	if (val == NULL) return;
	 
	if ((ctxt->sax != NULL) && (ctxt->sax->characters != NULL) &&
	    (!ctxt->disableSAX))
	    ctxt->sax->characters(ctxt->userData, val, xmlStrlen(val));
	return;
    }

     
    if (ent->checked == 0) {
	unsigned long oldnbent = ctxt->nbentities;

	 
	void *user_data;
	if (ctxt->userData == ctxt)
	    user_data = NULL;
	else
	    user_data = ctxt->userData;

	 
	if (ent->etype == XML_INTERNAL_GENERAL_ENTITY) {
	    ctxt->depth++;
	    ret = xmlParseBalancedChunkMemoryInternal(ctxt, ent->content,
	                                              user_data, &list);
	    ctxt->depth--;

	} else if (ent->etype == XML_EXTERNAL_GENERAL_PARSED_ENTITY) {
	    ctxt->depth++;
	    ret = xmlParseExternalEntityPrivate(ctxt->myDoc, ctxt, ctxt->sax,
	                                   user_data, ctxt->depth, ent->URI,
					   ent->ExternalID, &list);
	    ctxt->depth--;
	} else {
	    ret = XML_ERR_ENTITY_PE_INTERNAL;
	    xmlErrMsgStr(ctxt, XML_ERR_INTERNAL_ERROR,
			 "invalid entity type found\n", NULL);
	}

	 
	ent->checked = ctxt->nbentities - oldnbent;
	if (ret == XML_ERR_ENTITY_LOOP) {
	    xmlFatalErr(ctxt, XML_ERR_ENTITY_LOOP, NULL);
	    xmlFreeNodeList(list);
	    return;
	}
	if (xmlParserEntityCheck(ctxt, 0, ent)) {
	    xmlFreeNodeList(list);
	    return;
	}

	if ((ret == XML_ERR_OK) && (list != NULL)) {
	    if (((ent->etype == XML_INTERNAL_GENERAL_ENTITY) ||
	     (ent->etype == XML_EXTERNAL_GENERAL_PARSED_ENTITY))&&
		(ent->children == NULL)) {
		ent->children = list;
		if (ctxt->replaceEntities) {
		     
		    if (((list->type == XML_TEXT_NODE) &&
			 (list->next == NULL)) ||
			(ctxt->parseMode == XML_PARSE_READER)) {
			list->parent = (xmlNodePtr) ent;
			list = NULL;
			ent->owner = 1;
		    } else {
			ent->owner = 0;
			while (list != NULL) {
			    list->parent = (xmlNodePtr) ctxt->node;
			    list->doc = ctxt->myDoc;
			    if (list->next == NULL)
				ent->last = list;
			    list = list->next;
			}
			list = ent->children;
#ifdef LIBXML_LEGACY_ENABLED
			if (ent->etype == XML_EXTERNAL_GENERAL_PARSED_ENTITY)
			  xmlAddEntityReference(ent, list, NULL);
#endif  
		    }
		} else {
		    ent->owner = 1;
		    while (list != NULL) {
			list->parent = (xmlNodePtr) ent;
			if (list->next == NULL)
			    ent->last = list;
			list = list->next;
		    }
		}
	    } else {
		xmlFreeNodeList(list);
		list = NULL;
	    }
	} else if ((ret != XML_ERR_OK) &&
		   (ret != XML_WAR_UNDECLARED_ENTITY)) {
	    xmlFatalErrMsgStr(ctxt, XML_ERR_UNDECLARED_ENTITY,
		     "Entity '%s' failed to parse\n", ent->name);
	} else if (list != NULL) {
	    xmlFreeNodeList(list);
	    list = NULL;
	}
	if (ent->checked == 0)
	    ent->checked = 1;
    } else if (ent->checked != 1) {
	ctxt->nbentities += ent->checked;
    }

     
    if (ent->children == NULL) {
	 
	if (was_checked != 0) {
	    void *user_data;
	     
	    if (ctxt->userData == ctxt)
		user_data = NULL;
	    else
		user_data = ctxt->userData;

	    if (ent->etype == XML_INTERNAL_GENERAL_ENTITY) {
		ctxt->depth++;
		ret = xmlParseBalancedChunkMemoryInternal(ctxt,
				   ent->content, user_data, NULL);
		ctxt->depth--;
	    } else if (ent->etype ==
		       XML_EXTERNAL_GENERAL_PARSED_ENTITY) {
		ctxt->depth++;
		ret = xmlParseExternalEntityPrivate(ctxt->myDoc, ctxt,
			   ctxt->sax, user_data, ctxt->depth,
			   ent->URI, ent->ExternalID, NULL);
		ctxt->depth--;
	    } else {
		ret = XML_ERR_ENTITY_PE_INTERNAL;
		xmlErrMsgStr(ctxt, XML_ERR_INTERNAL_ERROR,
			     "invalid entity type found\n", NULL);
	    }
	    if (ret == XML_ERR_ENTITY_LOOP) {
		xmlFatalErr(ctxt, XML_ERR_ENTITY_LOOP, NULL);
		return;
	    }
	}
	if ((ctxt->sax != NULL) && (ctxt->sax->reference != NULL) &&
	    (ctxt->replaceEntities == 0) && (!ctxt->disableSAX)) {
	     
	    ctxt->sax->reference(ctxt->userData, ent->name);
	}
	return;
    }

     
    if ((ctxt->sax != NULL) && (ctxt->sax->reference != NULL) &&
	(ctxt->replaceEntities == 0) && (!ctxt->disableSAX)) {
	 
	ctxt->sax->reference(ctxt->userData, ent->name);
	return;
    }

    if ((ctxt->replaceEntities) || (ent->children == NULL))  {
	 
	if ((ctxt->node != NULL) && (ent->children != NULL)) {
	     
	    if (((list == NULL) && (ent->owner == 0)) ||
		(ctxt->parseMode == XML_PARSE_READER)) {
		xmlNodePtr nw = NULL, cur, firstChild = NULL;

		 

		cur = ent->children;
		while (cur != NULL) {
		    nw = xmlDocCopyNode(cur, ctxt->myDoc, 1);
		    if (nw != NULL) {
			if (nw->_private == NULL)
			    nw->_private = cur->_private;
			if (firstChild == NULL){
			    firstChild = nw;
			}
			nw = xmlAddChild(ctxt->node, nw);
		    }
		    if (cur == ent->last) {
			 
			if ((ctxt->parseMode == XML_PARSE_READER) &&
			    (nw != NULL) &&
			    (nw->type == XML_ELEMENT_NODE) &&
			    (nw->children == NULL))
			    nw->extra = 1;

			break;
		    }
		    cur = cur->next;
		}
#ifdef LIBXML_LEGACY_ENABLED
		if (ent->etype == XML_EXTERNAL_GENERAL_PARSED_ENTITY)
		  xmlAddEntityReference(ent, firstChild, nw);
#endif  
	    } else if (list == NULL) {
		xmlNodePtr nw = NULL, cur, next, last,
			   firstChild = NULL;
		 
		cur = ent->children;
		ent->children = NULL;
		last = ent->last;
		ent->last = NULL;
		while (cur != NULL) {
		    next = cur->next;
		    cur->next = NULL;
		    cur->parent = NULL;
		    nw = xmlDocCopyNode(cur, ctxt->myDoc, 1);
		    if (nw != NULL) {
			if (nw->_private == NULL)
			    nw->_private = cur->_private;
			if (firstChild == NULL){
			    firstChild = cur;
			}
			xmlAddChild((xmlNodePtr) ent, nw);
			xmlAddChild(ctxt->node, cur);
		    }
		    if (cur == last)
			break;
		    cur = next;
		}
		if (ent->owner == 0)
		    ent->owner = 1;
#ifdef LIBXML_LEGACY_ENABLED
		if (ent->etype == XML_EXTERNAL_GENERAL_PARSED_ENTITY)
		  xmlAddEntityReference(ent, firstChild, nw);
#endif  
	    } else {
		const xmlChar *nbktext;

		 
		nbktext = xmlDictLookup(ctxt->dict, BAD_CAST "nbktext",
					-1);
		if (ent->children->type == XML_TEXT_NODE)
		    ent->children->name = nbktext;
		if ((ent->last != ent->children) &&
		    (ent->last->type == XML_TEXT_NODE))
		    ent->last->name = nbktext;
		xmlAddChildList(ctxt->node, ent->children);
	    }

	     
	    ctxt->nodemem = 0;
	    ctxt->nodelen = 0;
	    return;
	}
    }
}

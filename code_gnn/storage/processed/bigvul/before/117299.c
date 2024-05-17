xmlXPtrEndPointFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr tmp, obj, point;
    xmlLocationSetPtr newset = NULL;
    xmlLocationSetPtr oldset = NULL;

    CHECK_ARITY(1);
    if ((ctxt->value == NULL) ||
	((ctxt->value->type != XPATH_LOCATIONSET) &&
	 (ctxt->value->type != XPATH_NODESET)))
        XP_ERROR(XPATH_INVALID_TYPE)

    obj = valuePop(ctxt);
    if (obj->type == XPATH_NODESET) {
	 
	tmp = xmlXPtrNewLocationSetNodeSet(obj->nodesetval);
	xmlXPathFreeObject(obj);
	obj = tmp;
    }

    newset = xmlXPtrLocationSetCreate(NULL);
    oldset = (xmlLocationSetPtr) obj->user;
    if (oldset != NULL) {
	int i;

	for (i = 0; i < oldset->locNr; i++) {
	    tmp = oldset->locTab[i];
	    if (tmp == NULL)
		continue;
	    point = NULL;
	    switch (tmp->type) {
		case XPATH_POINT:
		    point = xmlXPtrNewPoint(tmp->user, tmp->index);
		    break;
		case XPATH_RANGE: {
		    xmlNodePtr node = tmp->user2;
		    if (node != NULL) {
			if (node->type == XML_ATTRIBUTE_NODE) {
			     
			    xmlXPathFreeObject(obj);
			    xmlXPtrFreeLocationSet(newset);
			    XP_ERROR(XPTR_SYNTAX_ERROR);
			}
			point = xmlXPtrNewPoint(node, tmp->index2);
		    } else if (tmp->user == NULL) {
			point = xmlXPtrNewPoint(node,
				       xmlXPtrNbLocChildren(node));
		    }
		    break;
	        }
		default:
		     
		    break;
	    }
            if (point != NULL)
		xmlXPtrLocationSetAdd(newset, point);
	}
    }
    xmlXPathFreeObject(obj);
    valuePush(ctxt, xmlXPtrWrapLocationSet(newset));
}

xmlXPathCmpNodesExt(xmlNodePtr node1, xmlNodePtr node2) {
    int depth1, depth2;
    int misc = 0, precedence1 = 0, precedence2 = 0;
    xmlNodePtr miscNode1 = NULL, miscNode2 = NULL;
    xmlNodePtr cur, root;
    long l1, l2;

    if ((node1 == NULL) || (node2 == NULL))
	return(-2);

    if (node1 == node2)
	return(0);

     
    switch (node1->type) {
	case XML_ELEMENT_NODE:
	    if (node2->type == XML_ELEMENT_NODE) {
		if ((0 > (long) node1->content) &&  
		    (0 > (long) node2->content) &&
		    (node1->doc == node2->doc))
		{
		    l1 = -((long) node1->content);
		    l2 = -((long) node2->content);
		    if (l1 < l2)
			return(1);
		    if (l1 > l2)
			return(-1);
		} else
		    goto turtle_comparison;
	    }
	    break;
	case XML_ATTRIBUTE_NODE:
	    precedence1 = 1;  
	    miscNode1 = node1;
	    node1 = node1->parent;
	    misc = 1;
	    break;
	case XML_TEXT_NODE:
	case XML_CDATA_SECTION_NODE:
	case XML_COMMENT_NODE:
	case XML_PI_NODE: {
	    miscNode1 = node1;
	     
	    if (node1->prev != NULL) {
		do {
		    node1 = node1->prev;
		    if (node1->type == XML_ELEMENT_NODE) {
			precedence1 = 3;  
			break;
		    }
		    if (node1->prev == NULL) {
			precedence1 = 2;  
			 
			node1 = node1->parent;
			break;
		    }
		} while (1);
	    } else {
		precedence1 = 2;  
		node1 = node1->parent;
	    }
	    if ((node1 == NULL) || (node1->type != XML_ELEMENT_NODE) ||
		(0 <= (long) node1->content)) {
		 
		node1 = miscNode1;
		precedence1 = 0;
	    } else
		misc = 1;
	}
	    break;
	case XML_NAMESPACE_DECL:
	     
	    return(1);
	default:
	    break;
    }
    switch (node2->type) {
	case XML_ELEMENT_NODE:
	    break;
	case XML_ATTRIBUTE_NODE:
	    precedence2 = 1;  
	    miscNode2 = node2;
	    node2 = node2->parent;
	    misc = 1;
	    break;
	case XML_TEXT_NODE:
	case XML_CDATA_SECTION_NODE:
	case XML_COMMENT_NODE:
	case XML_PI_NODE: {
	    miscNode2 = node2;
	    if (node2->prev != NULL) {
		do {
		    node2 = node2->prev;
		    if (node2->type == XML_ELEMENT_NODE) {
			precedence2 = 3;  
			break;
		    }
		    if (node2->prev == NULL) {
			precedence2 = 2;  
			node2 = node2->parent;
			break;
		    }
		} while (1);
	    } else {
		precedence2 = 2;  
		node2 = node2->parent;
	    }
	    if ((node2 == NULL) || (node2->type != XML_ELEMENT_NODE) ||
		(0 <= (long) node1->content))
	    {
		node2 = miscNode2;
		precedence2 = 0;
	    } else
		misc = 1;
	}
	    break;
	case XML_NAMESPACE_DECL:
	    return(1);
	default:
	    break;
    }
    if (misc) {
	if (node1 == node2) {
	    if (precedence1 == precedence2) {
		 
		cur = miscNode2->prev;
		while (cur != NULL) {
		    if (cur == miscNode1)
			return(1);
		    if (cur->type == XML_ELEMENT_NODE)
			return(-1);
		    cur = cur->prev;
		}
		return (-1);
	    } else {
		 
		if (precedence1 < precedence2)
		    return(1);
		else
		    return(-1);
	    }
	}
	 
	if ((precedence2 == 3) && (precedence1 > 1)) {
	    cur = node1->parent;
	    while (cur) {
		if (cur == node2)
		    return(1);
		cur = cur->parent;
	    }
	}
	if ((precedence1 == 3) && (precedence2 > 1)) {
	    cur = node2->parent;
	    while (cur) {
		if (cur == node1)
		    return(-1);
		cur = cur->parent;
	    }
	}
    }

     
    if ((node1->type == XML_ELEMENT_NODE) &&
	(node2->type == XML_ELEMENT_NODE) &&
	(0 > (long) node1->content) &&
	(0 > (long) node2->content) &&
	(node1->doc == node2->doc)) {

	l1 = -((long) node1->content);
	l2 = -((long) node2->content);
	if (l1 < l2)
	    return(1);
	if (l1 > l2)
	    return(-1);
    }

turtle_comparison:

    if (node1 == node2->prev)
	return(1);
    if (node1 == node2->next)
	return(-1);
     
    for (depth2 = 0, cur = node2;cur->parent != NULL;cur = cur->parent) {
	if (cur == node1)
	    return(1);
	depth2++;
    }
    root = cur;
    for (depth1 = 0, cur = node1;cur->parent != NULL;cur = cur->parent) {
	if (cur == node2)
	    return(-1);
	depth1++;
    }
     
    if (root != cur) {
	return(-2);
    }
     
    while (depth1 > depth2) {
	depth1--;
	node1 = node1->parent;
    }
    while (depth2 > depth1) {
	depth2--;
	node2 = node2->parent;
    }
    while (node1->parent != node2->parent) {
	node1 = node1->parent;
	node2 = node2->parent;
	 
	if ((node1 == NULL) || (node2 == NULL))
	    return(-2);
    }
     
    if (node1 == node2->prev)
	return(1);
    if (node1 == node2->next)
	return(-1);
     
    if ((node1->type == XML_ELEMENT_NODE) &&
	(node2->type == XML_ELEMENT_NODE) &&
	(0 > (long) node1->content) &&
	(0 > (long) node2->content) &&
	(node1->doc == node2->doc)) {

	l1 = -((long) node1->content);
	l2 = -((long) node2->content);
	if (l1 < l2)
	    return(1);
	if (l1 > l2)
	    return(-1);
    }

    for (cur = node1->next;cur != NULL;cur = cur->next)
	if (cur == node2)
	    return(1);
    return(-1);  
}

xmlXPathCmpNodes(xmlNodePtr node1, xmlNodePtr node2) {
    int depth1, depth2;
    int attr1 = 0, attr2 = 0;
    xmlNodePtr attrNode1 = NULL, attrNode2 = NULL;
    xmlNodePtr cur, root;

    if ((node1 == NULL) || (node2 == NULL))
	return(-2);
     
    if (node1 == node2)		 
	return(0);
    if (node1->type == XML_ATTRIBUTE_NODE) {
	attr1 = 1;
	attrNode1 = node1;
	node1 = node1->parent;
    }
    if (node2->type == XML_ATTRIBUTE_NODE) {
	attr2 = 1;
	attrNode2 = node2;
	node2 = node2->parent;
    }
    if (node1 == node2) {
	if (attr1 == attr2) {
	     
	    if (attr1 != 0) {
	        cur = attrNode2->prev;
		while (cur != NULL) {
		    if (cur == attrNode1)
		        return (1);
		    cur = cur->prev;
		}
		return (-1);
	    }
	    return(0);
	}
	if (attr2 == 1)
	    return(1);
	return(-1);
    }
    if ((node1->type == XML_NAMESPACE_DECL) ||
        (node2->type == XML_NAMESPACE_DECL))
	return(1);
    if (node1 == node2->prev)
	return(1);
    if (node1 == node2->next)
	return(-1);

     
    if ((node1->type == XML_ELEMENT_NODE) &&
	(node2->type == XML_ELEMENT_NODE) &&
	(0 > (long) node1->content) &&
	(0 > (long) node2->content) &&
	(node1->doc == node2->doc)) {
	long l1, l2;

	l1 = -((long) node1->content);
	l2 = -((long) node2->content);
	if (l1 < l2)
	    return(1);
	if (l1 > l2)
	    return(-1);
    }

     
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
	long l1, l2;

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

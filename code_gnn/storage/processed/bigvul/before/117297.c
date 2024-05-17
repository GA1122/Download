xmlXPtrCmpPoints(xmlNodePtr node1, int index1, xmlNodePtr node2, int index2) {
    if ((node1 == NULL) || (node2 == NULL))
	return(-2);
     
    if (node1 == node2) {
	if (index1 < index2)
	    return(1);
	if (index1 > index2)
	    return(-1);
	return(0);
    }
    return(xmlXPathCmpNodes(node1, node2));
}

xmlXPtrGetStartPoint(xmlXPathObjectPtr obj, xmlNodePtr *node, int *indx) {
    if ((obj == NULL) || (node == NULL) || (indx == NULL))
	return(-1);

    switch (obj->type) {
        case XPATH_POINT:
	    *node = obj->user;
	    if (obj->index <= 0)
		*indx = 0;
	    else
		*indx = obj->index;
	    return(0);
        case XPATH_RANGE:
	    *node = obj->user;
	    if (obj->index <= 0)
		*indx = 0;
	    else
		*indx = obj->index;
	    return(0);
	default:
	    break;
    }
    return(-1);
}

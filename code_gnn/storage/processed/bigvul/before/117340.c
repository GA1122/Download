xmlXPtrRangesEqual(xmlXPathObjectPtr range1, xmlXPathObjectPtr range2) {
    if (range1 == range2)
	return(1);
    if ((range1 == NULL) || (range2 == NULL))
	return(0);
    if (range1->type != range2->type)
	return(0);
    if (range1->type != XPATH_RANGE)
	return(0);
    if (range1->user != range2->user)
	return(0);
    if (range1->index != range2->index)
	return(0);
    if (range1->user2 != range2->user2)
	return(0);
    if (range1->index2 != range2->index2)
	return(0);
    return(1);
}

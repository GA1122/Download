xmlXPathNodeSetClear(xmlNodeSetPtr set, int hasNsNodes)
{
    if ((set == NULL) || (set->nodeNr <= 0))
	return;
    else if (hasNsNodes) {
	int i;
	xmlNodePtr node;

	for (i = 0; i < set->nodeNr; i++) {
	    node = set->nodeTab[i];
	    if ((node != NULL) &&
		(node->type == XML_NAMESPACE_DECL))
		xmlXPathNodeSetFreeNs((xmlNsPtr) node);
	}
    }
    set->nodeNr = 0;
}

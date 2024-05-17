xmlXPathFreeValueTree(xmlNodeSetPtr obj) {
    int i;

    if (obj == NULL) return;

    if (obj->nodeTab != NULL) {
	for (i = 0;i < obj->nodeNr;i++) {
	    if (obj->nodeTab[i] != NULL) {
		if (obj->nodeTab[i]->type == XML_NAMESPACE_DECL) {
		    xmlXPathNodeSetFreeNs((xmlNsPtr) obj->nodeTab[i]);
		} else {
		    xmlFreeNodeList(obj->nodeTab[i]);
		}
	    }
	}
	xmlFree(obj->nodeTab);
    }
    xmlFree(obj);
}

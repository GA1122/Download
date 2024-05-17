xmlXPtrLocationSetAdd(xmlLocationSetPtr cur, xmlXPathObjectPtr val) {
    int i;

    if ((cur == NULL) || (val == NULL)) return;

     
    for (i = 0;i < cur->locNr;i++) {
	if (xmlXPtrRangesEqual(cur->locTab[i], val)) {
	    xmlXPathFreeObject(val);
	    return;
	}
    }

     
    if (cur->locMax == 0) {
        cur->locTab = (xmlXPathObjectPtr *) xmlMalloc(XML_RANGESET_DEFAULT *
					     sizeof(xmlXPathObjectPtr));
	if (cur->locTab == NULL) {
	    xmlXPtrErrMemory("adding location to set");
	    return;
	}
	memset(cur->locTab, 0 ,
	       XML_RANGESET_DEFAULT * (size_t) sizeof(xmlXPathObjectPtr));
        cur->locMax = XML_RANGESET_DEFAULT;
    } else if (cur->locNr == cur->locMax) {
        xmlXPathObjectPtr *temp;

        cur->locMax *= 2;
	temp = (xmlXPathObjectPtr *) xmlRealloc(cur->locTab, cur->locMax *
				      sizeof(xmlXPathObjectPtr));
	if (temp == NULL) {
	    xmlXPtrErrMemory("adding location to set");
	    return;
	}
	cur->locTab = temp;
    }
    cur->locTab[cur->locNr++] = val;
}

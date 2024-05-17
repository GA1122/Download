xmlXPtrFreeLocationSet(xmlLocationSetPtr obj) {
    int i;

    if (obj == NULL) return;
    if (obj->locTab != NULL) {
	for (i = 0;i < obj->locNr; i++) {
            xmlXPathFreeObject(obj->locTab[i]);
	}
	xmlFree(obj->locTab);
    }
    xmlFree(obj);
}

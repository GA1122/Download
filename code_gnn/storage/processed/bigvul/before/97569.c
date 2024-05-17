xmlXPathCacheFreeObjectList(xmlPointerListPtr list)
{
    int i;
    xmlXPathObjectPtr obj;

    if (list == NULL)
	return;

    for (i = 0; i < list->number; i++) {
	obj = list->items[i];
	 
	if (obj->nodesetval != NULL) {
	    if (obj->nodesetval->nodeTab != NULL)
		xmlFree(obj->nodesetval->nodeTab);
	    xmlFree(obj->nodesetval);
	}
	xmlFree(obj);
#ifdef XP_DEBUG_OBJ_USAGE
	xmlXPathDebugObjCounterAll--;
#endif
    }
    xmlPointerListFree(list);
}

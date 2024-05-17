xmlXPathFreeNodeSetList(xmlXPathObjectPtr obj) {
    if (obj == NULL) return;
#ifdef XP_DEBUG_OBJ_USAGE
    xmlXPathDebugObjUsageReleased(NULL, obj->type);
#endif
    xmlFree(obj);
}

xmlXPtrLocationSetDel(xmlLocationSetPtr cur, xmlXPathObjectPtr val) {
    int i;

    if (cur == NULL) return;
    if (val == NULL) return;

     
    for (i = 0;i < cur->locNr;i++)
        if (cur->locTab[i] == val) break;

    if (i >= cur->locNr) {
#ifdef DEBUG
        xmlGenericError(xmlGenericErrorContext, 
	        "xmlXPtrLocationSetDel: Range wasn't found in RangeList\n");
#endif
        return;
    }
    cur->locNr--;
    for (;i < cur->locNr;i++)
        cur->locTab[i] = cur->locTab[i + 1];
    cur->locTab[cur->locNr] = NULL;
}

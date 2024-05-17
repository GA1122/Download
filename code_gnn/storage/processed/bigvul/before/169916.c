xsltInitAllDocKeys(xsltTransformContextPtr ctxt)
{
    xsltStylesheetPtr style;
    xsltKeyDefPtr keyd;
    xsltKeyTablePtr table;

    if (ctxt == NULL)
	return(-1);

#ifdef KEY_INIT_DEBUG
fprintf(stderr, "xsltInitAllDocKeys %d %d\n",
        ctxt->document->nbKeysComputed, ctxt->nbKeys);
#endif

    if (ctxt->document->nbKeysComputed == ctxt->nbKeys)
	return(0);


     
    style = ctxt->style;
    while (style) {
	keyd = (xsltKeyDefPtr) style->keys;
	while (keyd != NULL) {
#ifdef KEY_INIT_DEBUG
fprintf(stderr, "Init key %s\n", keyd->name);
#endif
	     
	    table = (xsltKeyTablePtr) ctxt->document->keys;
	    while (table) {
		if (((keyd->nameURI != NULL) == (table->nameURI != NULL)) &&
		    xmlStrEqual(keyd->name, table->name) &&
		    xmlStrEqual(keyd->nameURI, table->nameURI))
		{
		    break;
		}
		table = table->next;
	    }
	    if (table == NULL) {
		 
		xsltInitDocKeyTable(ctxt, keyd->name, keyd->nameURI);
	    }
	    keyd = keyd->next;
	}
	style = xsltNextImport(style);
    }
#ifdef KEY_INIT_DEBUG
fprintf(stderr, "xsltInitAllDocKeys: done\n");
#endif
    return(0);
}
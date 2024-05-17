xmlXPathRegisteredVariablesCleanup(xmlXPathContextPtr ctxt) {
    if (ctxt == NULL)
	return;

    xmlHashFree(ctxt->varHash, (xmlHashDeallocator)xmlXPathFreeObject);
    ctxt->varHash = NULL;
}

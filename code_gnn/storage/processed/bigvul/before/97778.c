xmlXPathRegisteredNsCleanup(xmlXPathContextPtr ctxt) {
    if (ctxt == NULL)
	return;

    xmlHashFree(ctxt->nsHash, (xmlHashDeallocator)xmlFree);
    ctxt->nsHash = NULL;
}

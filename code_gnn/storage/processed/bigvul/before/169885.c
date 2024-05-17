xsltShutdownCtxtExts(xsltTransformContextPtr ctxt)
{
    if (ctxt == NULL)
        return;
    if (ctxt->extInfos == NULL)
        return;
    xmlHashScan(ctxt->extInfos, (xmlHashScanner) xsltShutdownCtxtExt,
                ctxt);
    xmlHashFree(ctxt->extInfos, (xmlHashDeallocator) xsltFreeExtData);
    ctxt->extInfos = NULL;
}

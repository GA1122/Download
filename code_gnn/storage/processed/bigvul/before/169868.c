xsltInitCtxtExts(xsltTransformContextPtr ctxt)
{
    xsltStylesheetPtr style;
    xsltInitExtCtxt ctx;

    if (ctxt == NULL)
        return (-1);

    style = ctxt->style;
    if (style == NULL)
        return (-1);

    ctx.ctxt = ctxt;
    ctx.ret = 0;

    while (style != NULL) {
        if (style->extInfos != NULL) {
            xmlHashScan(style->extInfos,
                        (xmlHashScanner) xsltInitCtxtExt, &ctx);
            if (ctx.ret == -1)
                return (-1);
        }
        style = xsltNextImport(style);
    }
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
    xsltGenericDebug(xsltGenericDebugContext, "Registered %d modules\n",
                     ctx.ret);
#endif
    return (ctx.ret);
}

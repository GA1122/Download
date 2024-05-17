templPop(xsltTransformContextPtr ctxt)
{
    xsltTemplatePtr ret;

    if (ctxt->templNr <= 0)
        return (0);
    ctxt->templNr--;
    if (ctxt->templNr > 0)
        ctxt->templ = ctxt->templTab[ctxt->templNr - 1];
    else
        ctxt->templ = (xsltTemplatePtr) 0;
    ret = ctxt->templTab[ctxt->templNr];
    ctxt->templTab[ctxt->templNr] = 0;
    return (ret);
}

profPop(xsltTransformContextPtr ctxt)
{
    long ret;

    if (ctxt->profNr <= 0)
        return (0);
    ctxt->profNr--;
    if (ctxt->profNr > 0)
        ctxt->prof = ctxt->profTab[ctxt->profNr - 1];
    else
        ctxt->prof = (long) 0;
    ret = ctxt->profTab[ctxt->profNr];
    ctxt->profTab[ctxt->profNr] = 0;
    return (ret);
}

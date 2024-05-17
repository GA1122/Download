xsltLocalVariablePop(xsltTransformContextPtr ctxt, int limitNr, int level)
{
    xsltStackElemPtr variable;

    if (ctxt->varsNr <= 0)
        return;

    do {
	if (ctxt->varsNr <= limitNr)
	    break;
	variable = ctxt->varsTab[ctxt->varsNr - 1];
	if (variable->level <= level)
	    break;
	if (variable->level >= 0)
	    xsltFreeStackElemList(variable);
	ctxt->varsNr--;
    } while (ctxt->varsNr != 0);
    if (ctxt->varsNr > 0)
        ctxt->vars = ctxt->varsTab[ctxt->varsNr - 1];
    else
        ctxt->vars = NULL;
}

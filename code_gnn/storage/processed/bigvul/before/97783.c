xmlXPathRoundFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    double f;

    CHECK_ARITY(1);
    CAST_TO_NUMBER;
    CHECK_TYPE(XPATH_NUMBER);

    if ((xmlXPathIsNaN(ctxt->value->floatval)) ||
	(xmlXPathIsInf(ctxt->value->floatval) == 1) ||
	(xmlXPathIsInf(ctxt->value->floatval) == -1) ||
	(ctxt->value->floatval == 0.0))
	return;

    XTRUNC(f, ctxt->value->floatval);
    if (ctxt->value->floatval < 0) {
	if (ctxt->value->floatval < f - 0.5)
	    ctxt->value->floatval = f - 1;
	else
	    ctxt->value->floatval = f;
	if (ctxt->value->floatval == 0)
	    ctxt->value->floatval = xmlXPathNZERO;
    } else {
	if (ctxt->value->floatval < f + 0.5)
	    ctxt->value->floatval = f;
	else
	    ctxt->value->floatval = f + 1;
    }
}

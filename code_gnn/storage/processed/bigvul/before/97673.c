xmlXPathFloorFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    double f;

    CHECK_ARITY(1);
    CAST_TO_NUMBER;
    CHECK_TYPE(XPATH_NUMBER);

    XTRUNC(f, ctxt->value->floatval);
    if (f != ctxt->value->floatval) {
	if (ctxt->value->floatval > 0)
	    ctxt->value->floatval = f;
	else
	    ctxt->value->floatval = f - 1;
    }
}

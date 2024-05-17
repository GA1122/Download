xmlXPathCeilingFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    double f;

    CHECK_ARITY(1);
    CAST_TO_NUMBER;
    CHECK_TYPE(XPATH_NUMBER);

#if 0
    ctxt->value->floatval = ceil(ctxt->value->floatval);
#else
    XTRUNC(f, ctxt->value->floatval);
    if (f != ctxt->value->floatval) {
	if (ctxt->value->floatval > 0)
	    ctxt->value->floatval = f + 1;
	else {
	    if (ctxt->value->floatval < 0 && f == 0)
	        ctxt->value->floatval = xmlXPathNZERO;
	    else
	        ctxt->value->floatval = f;
	}

    }
#endif
}

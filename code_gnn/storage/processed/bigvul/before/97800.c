xmlXPathValueFlipSign(xmlXPathParserContextPtr ctxt) {
    if ((ctxt == NULL) || (ctxt->context == NULL)) return;
    CAST_TO_NUMBER;
    CHECK_TYPE(XPATH_NUMBER);
    if (xmlXPathIsNaN(ctxt->value->floatval))
        ctxt->value->floatval=xmlXPathNAN;
    else if (xmlXPathIsInf(ctxt->value->floatval) == 1)
        ctxt->value->floatval=xmlXPathNINF;
    else if (xmlXPathIsInf(ctxt->value->floatval) == -1)
        ctxt->value->floatval=xmlXPathPINF;
    else if (ctxt->value->floatval == 0) {
        if (xmlXPathGetSign(ctxt->value->floatval) == 0)
	    ctxt->value->floatval = xmlXPathNZERO;
	else
	    ctxt->value->floatval = 0;
    }
    else
        ctxt->value->floatval = - ctxt->value->floatval;
}

xmlXPathDivValues(xmlXPathParserContextPtr ctxt) {
    xmlXPathObjectPtr arg;
    double val;

    arg = valuePop(ctxt);
    if (arg == NULL)
	XP_ERROR(XPATH_INVALID_OPERAND);
    val = xmlXPathCastToNumber(arg);
    xmlXPathReleaseObject(ctxt->context, arg);
    CAST_TO_NUMBER;
    CHECK_TYPE(XPATH_NUMBER);
    if (xmlXPathIsNaN(val) || xmlXPathIsNaN(ctxt->value->floatval))
	ctxt->value->floatval = xmlXPathNAN;
    else if (val == 0 && xmlXPathGetSign(val) != 0) {
	if (ctxt->value->floatval == 0)
	    ctxt->value->floatval = xmlXPathNAN;
	else if (ctxt->value->floatval > 0)
	    ctxt->value->floatval = xmlXPathNINF;
	else if (ctxt->value->floatval < 0)
	    ctxt->value->floatval = xmlXPathPINF;
    }
    else if (val == 0) {
	if (ctxt->value->floatval == 0)
	    ctxt->value->floatval = xmlXPathNAN;
	else if (ctxt->value->floatval > 0)
	    ctxt->value->floatval = xmlXPathPINF;
	else if (ctxt->value->floatval < 0)
	    ctxt->value->floatval = xmlXPathNINF;
    } else
	ctxt->value->floatval /= val;
}

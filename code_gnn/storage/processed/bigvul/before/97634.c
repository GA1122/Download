xmlXPathContainsFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr hay, needle;

    CHECK_ARITY(2);
    CAST_TO_STRING;
    CHECK_TYPE(XPATH_STRING);
    needle = valuePop(ctxt);
    CAST_TO_STRING;
    hay = valuePop(ctxt);

    if ((hay == NULL) || (hay->type != XPATH_STRING)) {
	xmlXPathReleaseObject(ctxt->context, hay);
	xmlXPathReleaseObject(ctxt->context, needle);
	XP_ERROR(XPATH_INVALID_TYPE);
    }
    if (xmlStrstr(hay->stringval, needle->stringval))
	valuePush(ctxt, xmlXPathCacheNewBoolean(ctxt->context, 1));
    else
	valuePush(ctxt, xmlXPathCacheNewBoolean(ctxt->context, 0));
    xmlXPathReleaseObject(ctxt->context, hay);
    xmlXPathReleaseObject(ctxt->context, needle);
}

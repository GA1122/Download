_exsltDateParseGMonth (exsltDateValDatePtr dt, const xmlChar **str)
{
    const xmlChar *cur = *str;
    int ret = 0;

    PARSE_2_DIGITS(dt->mon, cur, VALID_MONTH, ret);
    if (ret != 0)
	return ret;

    *str = cur;

#ifdef DEBUG_EXSLT_DATE
    xsltGenericDebug(xsltGenericDebugContext,
		     "Parsed month %02i\n", dt->mon);
#endif

    return 0;
}

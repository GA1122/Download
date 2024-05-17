_exsltDateParseGDay (exsltDateValDatePtr dt, const xmlChar **str)
{
    const xmlChar *cur = *str;
    int ret = 0;

    PARSE_2_DIGITS(dt->day, cur, VALID_DAY, ret);
    if (ret != 0)
	return ret;

    *str = cur;

#ifdef DEBUG_EXSLT_DATE
    xsltGenericDebug(xsltGenericDebugContext,
		     "Parsed day %02i\n", dt->day);
#endif

    return 0;
}

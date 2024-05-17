_exsltDateParseTime (exsltDateValDatePtr dt, const xmlChar **str)
{
    const xmlChar *cur = *str;
    unsigned int hour = 0;  
    int ret = 0;

    PARSE_2_DIGITS(hour, cur, VALID_HOUR, ret);
    if (ret != 0)
	return ret;

    if (*cur != ':')
	return 1;
    cur++;

     
    dt->hour = hour;

    PARSE_2_DIGITS(dt->min, cur, VALID_MIN, ret);
    if (ret != 0)
	return ret;

    if (*cur != ':')
	return 1;
    cur++;

    PARSE_FLOAT(dt->sec, cur, ret);
    if (ret != 0)
	return ret;

    if (!VALID_TIME(dt))
	return 2;

    *str = cur;

#ifdef DEBUG_EXSLT_DATE
    xsltGenericDebug(xsltGenericDebugContext,
		     "Parsed time %02i:%02i:%02.f\n",
		     dt->hour, dt->min, dt->sec);
#endif

    return 0;
}

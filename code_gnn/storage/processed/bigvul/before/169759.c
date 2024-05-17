_exsltDateParseTimeZone (exsltDateValDatePtr dt, const xmlChar **str)
{
    const xmlChar *cur;
    int ret = 0;

    if (str == NULL)
	return -1;
    cur = *str;
    switch (*cur) {
    case 0:
	dt->tz_flag = 0;
	dt->tzo = 0;
	break;

    case 'Z':
	dt->tz_flag = 1;
	dt->tzo = 0;
	cur++;
	break;

    case '+':
    case '-': {
	int isneg = 0, tmp = 0;
	isneg = (*cur == '-');

	cur++;

	PARSE_2_DIGITS(tmp, cur, VALID_HOUR, ret);
	if (ret != 0)
	    return ret;

	if (*cur != ':')
	    return 1;
	cur++;

	dt->tzo = tmp * 60;

	PARSE_2_DIGITS(tmp, cur, VALID_MIN, ret);
	if (ret != 0)
	    return ret;

	dt->tzo += tmp;
	if (isneg)
	    dt->tzo = - dt->tzo;

	if (!VALID_TZO(dt->tzo))
	    return 2;

	break;
      }
    default:
	return 1;
    }

    *str = cur;

#ifdef DEBUG_EXSLT_DATE
    xsltGenericDebug(xsltGenericDebugContext,
		     "Parsed time zone offset (%s) %i\n",
		     dt->tz_flag ? "explicit" : "implicit", dt->tzo);
#endif

    return 0;
}

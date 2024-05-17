exsltDateParse (const xmlChar *dateTime)
{
    exsltDateValPtr dt;
    int ret;
    const xmlChar *cur = dateTime;

#define RETURN_TYPE_IF_VALID(t)					\
    if (IS_TZO_CHAR(*cur)) {					\
	ret = _exsltDateParseTimeZone(&(dt->value.date), &cur);	\
	if (ret == 0) {						\
	    if (*cur != 0)					\
		goto error;					\
	    dt->type = t;					\
	    return dt;						\
	}							\
    }

    if (dateTime == NULL)
	return NULL;

    if ((*cur != '-') && (*cur < '0') && (*cur > '9'))
	return NULL;

    dt = exsltDateCreateDate(EXSLT_UNKNOWN);
    if (dt == NULL)
	return NULL;

    if ((cur[0] == '-') && (cur[1] == '-')) {
	 
	cur += 2;

	 
	if (*cur == '-') {
	  ++cur;
	    ret = _exsltDateParseGDay(&(dt->value.date), &cur);
	    if (ret != 0)
		goto error;

	    RETURN_TYPE_IF_VALID(XS_GDAY);

	    goto error;
	}

	 
	ret = _exsltDateParseGMonth(&(dt->value.date), &cur);
	if (ret != 0)
	    goto error;

	if (*cur != '-')
	    goto error;
	cur++;

	 
	if (*cur == '-') {
	    cur++;
	    RETURN_TYPE_IF_VALID(XS_GMONTH);
	    goto error;
	}

	 
	ret = _exsltDateParseGDay(&(dt->value.date), &cur);
	if (ret != 0)
	    goto error;

	RETURN_TYPE_IF_VALID(XS_GMONTHDAY);

	goto error;
    }

     
    if ((*cur >= '0') && (*cur <= '9')) {
	ret = _exsltDateParseTime(&(dt->value.date), &cur);
	if (ret == 0) {
	     
	    RETURN_TYPE_IF_VALID(XS_TIME);
	}
    }

     
    cur = dateTime;

    ret = _exsltDateParseGYear(&(dt->value.date), &cur);
    if (ret != 0)
	goto error;

     
    RETURN_TYPE_IF_VALID(XS_GYEAR);

    if (*cur != '-')
	goto error;
    cur++;

    ret = _exsltDateParseGMonth(&(dt->value.date), &cur);
    if (ret != 0)
	goto error;

     
    RETURN_TYPE_IF_VALID(XS_GYEARMONTH);

    if (*cur != '-')
	goto error;
    cur++;

    ret = _exsltDateParseGDay(&(dt->value.date), &cur);
    if ((ret != 0) || !VALID_DATE((&(dt->value.date))))
	goto error;

     
    RETURN_TYPE_IF_VALID(XS_DATE);

    if (*cur != 'T')
	goto error;
    cur++;

     
    ret = _exsltDateParseTime(&(dt->value.date), &cur);
    if (ret != 0)
	goto error;

    ret = _exsltDateParseTimeZone(&(dt->value.date), &cur);
    if ((ret != 0) || (*cur != 0) || !VALID_DATETIME((&(dt->value.date))))
	goto error;

    dt->type = XS_DATETIME;

    return dt;

error:
    if (dt != NULL)
	exsltDateFreeDate(dt);
    return NULL;
}

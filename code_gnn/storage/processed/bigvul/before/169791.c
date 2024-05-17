exsltDateMonthInYear (const xmlChar *dateTime)
{
    exsltDateValPtr dt;
    double ret;

    if (dateTime == NULL) {
#ifdef WITH_TIME
	dt = exsltDateCurrent();
	if (dt == NULL)
#endif
	    return xmlXPathNAN;
    } else {
	dt = exsltDateParse(dateTime);
	if (dt == NULL)
	    return xmlXPathNAN;
	if ((dt->type != XS_DATETIME) && (dt->type != XS_DATE) &&
	    (dt->type != XS_GYEARMONTH) && (dt->type != XS_GMONTH) &&
	    (dt->type != XS_GMONTHDAY)) {
	    exsltDateFreeDate(dt);
	    return xmlXPathNAN;
	}
    }

    ret = (double) dt->value.date.mon;
    exsltDateFreeDate(dt);

    return ret;
}

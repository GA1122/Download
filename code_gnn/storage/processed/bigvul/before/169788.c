exsltDateMinuteInHour (const xmlChar *dateTime)
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
	if ((dt->type != XS_DATETIME) && (dt->type != XS_TIME)) {
	    exsltDateFreeDate(dt);
	    return xmlXPathNAN;
	}
    }

    ret = (double) dt->value.date.min;
    exsltDateFreeDate(dt);

    return ret;
}

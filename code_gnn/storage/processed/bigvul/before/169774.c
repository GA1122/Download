exsltDateDayOfWeekInMonth (const xmlChar *dateTime)
{
    exsltDateValPtr dt;
    long ret;

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
	if ((dt->type != XS_DATETIME) && (dt->type != XS_DATE)) {
	    exsltDateFreeDate(dt);
	    return xmlXPathNAN;
	}
    }

    ret = ((dt->value.date.day -1) / 7) + 1;

    exsltDateFreeDate(dt);

    return (double) ret;
}

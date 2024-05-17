exsltDateDayInYear (const xmlChar *dateTime)
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

    ret = DAY_IN_YEAR(dt->value.date.day, dt->value.date.mon,
                      dt->value.date.year);

    exsltDateFreeDate(dt);

    return (double) ret;
}

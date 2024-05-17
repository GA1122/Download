exsltDateWeekInMonth (const xmlChar *dateTime)
{
    exsltDateValPtr dt;
    long fdiy, fdiw, ret;

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

    fdiy = DAY_IN_YEAR(1, dt->value.date.mon, dt->value.date.year);
     
    fdiw = (_exsltDateDayInWeek(fdiy, dt->value.date.year) + 6) % 7;

    ret = ((dt->value.date.day + fdiw - 1) / 7) + 1;

    exsltDateFreeDate(dt);

    return (double) ret;
}

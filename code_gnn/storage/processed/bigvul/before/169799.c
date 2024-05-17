exsltDateSeconds (const xmlChar *dateTime)
{
    exsltDateValPtr dt;
    double ret = xmlXPathNAN;

    if (dateTime == NULL) {
#ifdef WITH_TIME
	dt = exsltDateCurrent();
	if (dt == NULL)
#endif
	    return xmlXPathNAN;
    } else {
        dt = exsltDateParseDuration(dateTime);
        if (dt == NULL)
            dt = exsltDateParse(dateTime);
    }

    if (dt == NULL)
        return xmlXPathNAN;

    if ((dt->type <= XS_DATETIME) && (dt->type >= XS_GYEAR)) {
        exsltDateValPtr y, dur;

         
        y = exsltDateCreateDate(XS_DATETIME);
        if (y != NULL) {
            y->value.date.year = 1970;
            y->value.date.mon  = 1;
            y->value.date.day  = 1;
            y->value.date.tz_flag = 1;

            dur = _exsltDateDifference(y, dt, 1);
            if (dur != NULL) {
                ret = exsltDateCastDateToNumber(dur);
                exsltDateFreeDate(dur);
            }
            exsltDateFreeDate(y);
        }

    } else if ((dt->type == XS_DURATION) && (dt->value.dur.mon == 0))
        ret = exsltDateCastDateToNumber(dt);

    exsltDateFreeDate(dt);

    return ret;
}

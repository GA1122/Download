exsltDateTime (const xmlChar *dateTime)
{
    exsltDateValPtr dt = NULL;
    xmlChar *ret = NULL;

    if (dateTime == NULL) {
#ifdef WITH_TIME
	dt = exsltDateCurrent();
	if (dt == NULL)
#endif
	    return NULL;
    } else {
	dt = exsltDateParse(dateTime);
	if (dt == NULL)
	    return NULL;
	if ((dt->type != XS_DATETIME) && (dt->type != XS_TIME)) {
	    exsltDateFreeDate(dt);
	    return NULL;
	}
    }

    ret = exsltDateFormatTime(&(dt->value.date));
    exsltDateFreeDate(dt);

    return ret;
}
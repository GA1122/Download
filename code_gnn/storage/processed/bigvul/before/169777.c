exsltDateDuration (const xmlChar *number)
{
    exsltDateValPtr dur;
    double       secs;
    xmlChar     *ret;

    if (number == NULL)
        secs = exsltDateSeconds(number);
    else
        secs = xmlXPathCastStringToNumber(number);

    if ((xmlXPathIsNaN(secs)) || (xmlXPathIsInf(secs)))
        return NULL;

    dur = exsltDateCreateDate(XS_DURATION);
    if (dur == NULL)
        return NULL;

    dur->value.dur.sec = secs;

    ret = exsltDateFormatDuration(&(dur->value.dur));
    exsltDateFreeDate(dur);

    return ret;
}

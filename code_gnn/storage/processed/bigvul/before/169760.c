exsltDateAdd (const xmlChar *xstr, const xmlChar *ystr)
{
    exsltDateValPtr dt, dur, res;
    xmlChar     *ret;

    if ((xstr == NULL) || (ystr == NULL))
        return NULL;

    dt = exsltDateParse(xstr);
    if (dt == NULL)
        return NULL;
    else if ((dt->type < XS_GYEAR) || (dt->type > XS_DATETIME)) {
        exsltDateFreeDate(dt);
        return NULL;
    }

    dur = exsltDateParseDuration(ystr);
    if (dur == NULL) {
        exsltDateFreeDate(dt);
        return NULL;
    }

    res = _exsltDateAdd(dt, dur);

    exsltDateFreeDate(dt);
    exsltDateFreeDate(dur);

    if (res == NULL)
        return NULL;

    ret = exsltDateFormat(res);
    exsltDateFreeDate(res);

    return ret;
}

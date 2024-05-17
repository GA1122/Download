exsltDateDifference (const xmlChar *xstr, const xmlChar *ystr)
{
    exsltDateValPtr x, y, dur;
    xmlChar        *ret = NULL;

    if ((xstr == NULL) || (ystr == NULL))
        return NULL;

    x = exsltDateParse(xstr);
    if (x == NULL)
        return NULL;

    y = exsltDateParse(ystr);
    if (y == NULL) {
        exsltDateFreeDate(x);
        return NULL;
    }

    if (((x->type < XS_GYEAR) || (x->type > XS_DATETIME)) ||
        ((y->type < XS_GYEAR) || (y->type > XS_DATETIME)))  {
	exsltDateFreeDate(x);
	exsltDateFreeDate(y);
        return NULL;
    }

    dur = _exsltDateDifference(x, y, 0);

    exsltDateFreeDate(x);
    exsltDateFreeDate(y);

    if (dur == NULL)
        return NULL;

    ret = exsltDateFormatDuration(&(dur->value.dur));
    exsltDateFreeDate(dur);

    return ret;
}

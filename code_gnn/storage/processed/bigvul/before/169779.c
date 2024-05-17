exsltDateFormat (const exsltDateValPtr dt)
{

    if (dt == NULL)
	return NULL;

    switch (dt->type) {
    case XS_DURATION:
        return exsltDateFormatDuration(&(dt->value.dur));
    case XS_DATETIME:
        return exsltDateFormatDateTime(&(dt->value.date));
    case XS_DATE:
        return exsltDateFormatDate(&(dt->value.date));
    case XS_TIME:
        return exsltDateFormatTime(&(dt->value.date));
    default:
        break;
    }

    if (dt->type & XS_GYEAR) {
        xmlChar buf[20], *cur = buf;

        FORMAT_GYEAR(dt->value.date.year, cur);
        if (dt->type == XS_GYEARMONTH) {
	    *cur = '-';
	    cur++;
	    FORMAT_GMONTH(dt->value.date.mon, cur);
        }

        if (dt->value.date.tz_flag || (dt->value.date.tzo != 0)) {
	    FORMAT_TZ(dt->value.date.tzo, cur);
        }
        *cur = 0;
        return xmlStrdup(buf);
    }

    return NULL;
}

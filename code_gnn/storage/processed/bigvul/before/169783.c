exsltDateFormatTime (const exsltDateValDatePtr dt)
{
    xmlChar buf[100], *cur = buf;

    if ((dt == NULL) || !VALID_TIME(dt))
	return NULL;

    FORMAT_TIME(dt, cur);
    if (dt->tz_flag || (dt->tzo != 0)) {
	FORMAT_TZ(dt->tzo, cur);
    }
    *cur = 0;

    return xmlStrdup(buf);
}

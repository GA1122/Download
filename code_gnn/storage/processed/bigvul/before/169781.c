exsltDateFormatDateTime (const exsltDateValDatePtr dt)
{
    xmlChar buf[100], *cur = buf;

    if ((dt == NULL) ||	!VALID_DATETIME(dt))
	return NULL;

    FORMAT_DATE(dt, cur);
    *cur = 'T';
    cur++;
    FORMAT_TIME(dt, cur);
    FORMAT_TZ(dt->tzo, cur);
    *cur = 0;

    return xmlStrdup(buf);
}

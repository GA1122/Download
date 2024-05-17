exsltDateFormatDuration (const exsltDateValDurationPtr dt)
{
    xmlChar buf[100], *cur = buf;
    double secs, days;
    double years, months;

    if (dt == NULL)
	return NULL;

     
    if ((dt->sec == 0.0) && (dt->day == 0) && (dt->mon == 0))
        return xmlStrdup((xmlChar*)"P0D");

    secs   = dt->sec;
    days   = (double)dt->day;
    years  = (double)(dt->mon / 12);
    months = (double)(dt->mon % 12);

    *cur = '\0';
    if (secs < 0.0) {
        secs = -secs;
        *cur = '-';
    }
    if (days < 0) {
        days = -days;
        *cur = '-';
    }
    if (years < 0) {
        years = -years;
        *cur = '-';
    }
    if (months < 0) {
        months = -months;
        *cur = '-';
    }
    if (*cur == '-')
	cur++;

    *cur++ = 'P';

    if (years != 0.0) {
        FORMAT_ITEM(years, cur, 1, 'Y');
    }

    if (months != 0.0) {
        FORMAT_ITEM(months, cur, 1, 'M');
    }

    if (secs >= SECS_PER_DAY) {
        double tmp = floor(secs / SECS_PER_DAY);
        days += tmp;
        secs -= (tmp * SECS_PER_DAY);
    }

    FORMAT_ITEM(days, cur, 1, 'D');
    if (secs > 0.0) {
        *cur++ = 'T';
    }
    FORMAT_ITEM(secs, cur, SECS_PER_HOUR, 'H');
    FORMAT_ITEM(secs, cur, SECS_PER_MIN, 'M');
    if (secs > 0.0) {
        FORMAT_FLOAT(secs, cur, 0);
        *cur++ = 'S';
    }

    *cur = 0;

    return xmlStrdup(buf);
}

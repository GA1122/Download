exsltDateCurrent (void)
{
    struct tm localTm, gmTm;
    time_t secs;
    int local_s, gm_s;
    exsltDateValPtr ret;

    ret = exsltDateCreateDate(XS_DATETIME);
    if (ret == NULL)
        return NULL;

     
    secs    = time(NULL);
#if HAVE_LOCALTIME_R
    localtime_r(&secs, &localTm);
#else
    localTm = *localtime(&secs);
#endif

     
    ret->value.date.year = localTm.tm_year + 1900;

    ret->value.date.mon  = localTm.tm_mon + 1;
    ret->value.date.day  = localTm.tm_mday;
    ret->value.date.hour = localTm.tm_hour;
    ret->value.date.min  = localTm.tm_min;

     
    ret->value.date.sec  = (double) localTm.tm_sec;

     
#if HAVE_GMTIME_R
    gmtime_r(&secs, &gmTm);
#else
    gmTm = *gmtime(&secs);
#endif
    ret->value.date.tz_flag = 0;
#if 0
    ret->value.date.tzo = (((ret->value.date.day * 1440) +
                            (ret->value.date.hour * 60) +
                             ret->value.date.min) -
                           ((gmTm.tm_mday * 1440) + (gmTm.tm_hour * 60) +
                             gmTm.tm_min));
#endif
    local_s = localTm.tm_hour * SECS_PER_HOUR +
        localTm.tm_min * SECS_PER_MIN +
        localTm.tm_sec;

    gm_s = gmTm.tm_hour * SECS_PER_HOUR +
        gmTm.tm_min * SECS_PER_MIN +
        gmTm.tm_sec;

    if (localTm.tm_year < gmTm.tm_year) {
	ret->value.date.tzo = -((SECS_PER_DAY - local_s) + gm_s)/60;
    } else if (localTm.tm_year > gmTm.tm_year) {
	ret->value.date.tzo = ((SECS_PER_DAY - gm_s) + local_s)/60;
    } else if (localTm.tm_mon < gmTm.tm_mon) {
	ret->value.date.tzo = -((SECS_PER_DAY - local_s) + gm_s)/60;
    } else if (localTm.tm_mon > gmTm.tm_mon) {
	ret->value.date.tzo = ((SECS_PER_DAY - gm_s) + local_s)/60;
    } else if (localTm.tm_mday < gmTm.tm_mday) {
	ret->value.date.tzo = -((SECS_PER_DAY - local_s) + gm_s)/60;
    } else if (localTm.tm_mday > gmTm.tm_mday) {
	ret->value.date.tzo = ((SECS_PER_DAY - gm_s) + local_s)/60;
    } else  {
	ret->value.date.tzo = (local_s - gm_s)/60;
    }

    return ret;
}

time_from_tm(struct tm *t)
{
#if HAVE_TIMEGM
         
        return (timegm(t));
#elif HAVE__MKGMTIME64
        return (_mkgmtime64(t));
#else
         
         
        if (mktime(t) == (time_t)-1)
                return ((time_t)-1);
         
        return (t->tm_sec
            + t->tm_min * 60
            + t->tm_hour * 3600
            + t->tm_yday * 86400
            + (t->tm_year - 70) * 31536000
            + ((t->tm_year - 69) / 4) * 86400
            - ((t->tm_year - 1) / 100) * 86400
            + ((t->tm_year + 299) / 400) * 86400);
#endif
}

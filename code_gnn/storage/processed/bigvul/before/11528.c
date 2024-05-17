char *format_timestamp(char *buf, size_t l, usec_t t) {
        struct tm tm;
        time_t sec;

        assert(buf);
        assert(l > 0);

        if (t <= 0)
                return NULL;

        sec = (time_t) (t / USEC_PER_SEC);

        if (strftime(buf, l, "%a, %d %b %Y %H:%M:%S %z", localtime_r(&sec, &tm)) <= 0)
                return NULL;

        return buf;
}

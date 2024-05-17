char *format_timestamp_pretty(char *buf, size_t l, usec_t t) {
        usec_t n, d;

        n = now(CLOCK_REALTIME);

        if (t <= 0 || t > n || t + USEC_PER_DAY*7 <= t)
                return NULL;

        d = n - t;

        if (d >= USEC_PER_YEAR)
                snprintf(buf, l, "%llu years and %llu months ago",
                         (unsigned long long) (d / USEC_PER_YEAR),
                         (unsigned long long) ((d % USEC_PER_YEAR) / USEC_PER_MONTH));
        else if (d >= USEC_PER_MONTH)
                snprintf(buf, l, "%llu months and %llu days ago",
                         (unsigned long long) (d / USEC_PER_MONTH),
                         (unsigned long long) ((d % USEC_PER_MONTH) / USEC_PER_DAY));
        else if (d >= USEC_PER_WEEK)
                snprintf(buf, l, "%llu weeks and %llu days ago",
                         (unsigned long long) (d / USEC_PER_WEEK),
                         (unsigned long long) ((d % USEC_PER_WEEK) / USEC_PER_DAY));
        else if (d >= 2*USEC_PER_DAY)
                snprintf(buf, l, "%llu days ago", (unsigned long long) (d / USEC_PER_DAY));
        else if (d >= 25*USEC_PER_HOUR)
                snprintf(buf, l, "1 day and %lluh ago",
                         (unsigned long long) ((d - USEC_PER_DAY) / USEC_PER_HOUR));
        else if (d >= 6*USEC_PER_HOUR)
                snprintf(buf, l, "%lluh ago",
                         (unsigned long long) (d / USEC_PER_HOUR));
        else if (d >= USEC_PER_HOUR)
                snprintf(buf, l, "%lluh %llumin ago",
                         (unsigned long long) (d / USEC_PER_HOUR),
                         (unsigned long long) ((d % USEC_PER_HOUR) / USEC_PER_MINUTE));
        else if (d >= 5*USEC_PER_MINUTE)
                snprintf(buf, l, "%llumin ago",
                         (unsigned long long) (d / USEC_PER_MINUTE));
        else if (d >= USEC_PER_MINUTE)
                snprintf(buf, l, "%llumin %llus ago",
                         (unsigned long long) (d / USEC_PER_MINUTE),
                         (unsigned long long) ((d % USEC_PER_MINUTE) / USEC_PER_SEC));
        else if (d >= USEC_PER_SEC)
                snprintf(buf, l, "%llus ago",
                         (unsigned long long) (d / USEC_PER_SEC));
        else if (d >= USEC_PER_MSEC)
                snprintf(buf, l, "%llums ago",
                         (unsigned long long) (d / USEC_PER_MSEC));
        else if (d > 0)
                snprintf(buf, l, "%lluus ago",
                         (unsigned long long) d);
        else
                snprintf(buf, l, "now");

        buf[l-1] = 0;
        return buf;
}

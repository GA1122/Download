int parse_usec(const char *t, usec_t *usec) {
        static const struct {
                const char *suffix;
                usec_t usec;
        } table[] = {
                { "sec", USEC_PER_SEC },
                { "s", USEC_PER_SEC },
                { "min", USEC_PER_MINUTE },
                { "hr", USEC_PER_HOUR },
                { "h", USEC_PER_HOUR },
                { "d", USEC_PER_DAY },
                { "w", USEC_PER_WEEK },
                { "msec", USEC_PER_MSEC },
                { "ms", USEC_PER_MSEC },
                { "m", USEC_PER_MINUTE },
                { "usec", 1ULL },
                { "us", 1ULL },
                { "", USEC_PER_SEC },
        };

        const char *p;
        usec_t r = 0;

        assert(t);
        assert(usec);

        p = t;
        do {
                long long l;
                char *e;
                unsigned i;

                errno = 0;
                l = strtoll(p, &e, 10);

                if (errno != 0)
                        return -errno;

                if (l < 0)
                        return -ERANGE;

                if (e == p)
                        return -EINVAL;

                e += strspn(e, WHITESPACE);

                for (i = 0; i < ELEMENTSOF(table); i++)
                        if (startswith(e, table[i].suffix)) {
                                r += (usec_t) l * table[i].usec;
                                p = e + strlen(table[i].suffix);
                                break;
                        }

                if (i >= ELEMENTSOF(table))
                        return -EINVAL;

        } while (*p != 0);

        *usec = r;

        return 0;
}

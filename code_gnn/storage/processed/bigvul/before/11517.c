dual_timestamp* dual_timestamp_from_realtime(dual_timestamp *ts, usec_t u) {
        int64_t delta;
        assert(ts);

        ts->realtime = u;

        if (u == 0)
                ts->monotonic = 0;
        else {
                delta = (int64_t) now(CLOCK_REALTIME) - (int64_t) u;

                ts->monotonic = now(CLOCK_MONOTONIC);

                if ((int64_t) ts->monotonic > delta)
                        ts->monotonic -= delta;
                else
                        ts->monotonic = 0;
        }

        return ts;
}

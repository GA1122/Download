size_t page_size(void) {
        static __thread size_t pgsz = 0;
        long r;

        if (_likely_(pgsz > 0))
                return pgsz;

        assert_se((r = sysconf(_SC_PAGESIZE)) > 0);

        pgsz = (size_t) r;

        return pgsz;
}

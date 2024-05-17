DEBUG_TRACE_FUNC(const char *func, unsigned line, const char *fmt, ...)
{
	va_list args;
	uint64_t nsnow;
	static uint64_t nslast;
	struct timespec tsnow;

	 
	unsigned long thread_id = mg_current_thread_id();

	clock_gettime(CLOCK_REALTIME, &tsnow);
	nsnow = ((uint64_t)tsnow.tv_sec) * ((uint64_t)1000000000)
	        + ((uint64_t)tsnow.tv_nsec);

	if (!nslast) {
		nslast = nsnow;
	}

	flockfile(stdout);
	printf("*** %lu.%09lu %12" INT64_FMT " %lu %s:%u: ",
	       (unsigned long)tsnow.tv_sec,
	       (unsigned long)tsnow.tv_nsec,
	       nsnow - nslast,
	       thread_id,
	       func,
	       line);
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	putchar('\n');
	fflush(stdout);
	funlockfile(stdout);
	nslast = nsnow;
}
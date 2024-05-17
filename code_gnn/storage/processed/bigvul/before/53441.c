iprintf(struct iperf_test *test, const char* format, ...)
{
    va_list argp;
    int r = -1;

     
    if (test->role == 'c') {
	if (test->title)
	    printf("%s:  ", test->title);
	va_start(argp, format);
	r = vprintf(format, argp);
	va_end(argp);
    }
    else if (test->role == 's') {
	char linebuffer[1024];
	va_start(argp, format);
	r = vsnprintf(linebuffer, sizeof(linebuffer), format, argp);
	va_end(argp);
	printf("%s", linebuffer);

	if (test->role == 's' && iperf_get_test_get_server_output(test)) {
	    struct iperf_textline *l = (struct iperf_textline *) malloc(sizeof(struct iperf_textline));
	    l->line = strdup(linebuffer);
	    TAILQ_INSERT_TAIL(&(test->server_output_list), l, textlineentries);
	}
    }
    return r;
}

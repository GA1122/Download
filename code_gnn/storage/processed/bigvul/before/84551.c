wrap_GC_warn_proc(char *msg, GC_word arg)
{
    if (fmInitialized) {
	 
	static struct {
	    char *msg;
	    GC_word arg;
	} msg_ring[GC_WARN_KEEP_MAX];
	 
	static int i = 0;
	static int n = 0;
	static int lock = 0;
	int j;

	j = (i + n) % (sizeof(msg_ring) / sizeof(msg_ring[0]));
	msg_ring[j].msg = msg;
	msg_ring[j].arg = arg;

	if (n < sizeof(msg_ring) / sizeof(msg_ring[0]))
	    ++n;
	else
	    ++i;

	if (!lock) {
	    lock = 1;

	    for (; n > 0; --n, ++i) {
		i %= sizeof(msg_ring) / sizeof(msg_ring[0]);

		printf(msg_ring[i].msg,	(unsigned long)msg_ring[i].arg);
		sleep_till_anykey(1, 1);
	    }

	    lock = 0;
	}
    }
    else if (orig_GC_warn_proc)
	orig_GC_warn_proc(msg, arg);
    else
	fprintf(stderr, msg, (unsigned long)arg);
}
static int check_excludes(struct perf_event **evts, int n_prev, int n_new)
{
	int eu = 0, ek = 0, eh = 0;
	struct perf_event *event;
	int i, n, first;

	n = n_prev + n_new;
	if (n <= 1)
		return 0;

	first = 1;
	for (i = 0; i < n; i++) {
		event = evts[i];
		if (first) {
			eu = event->attr.exclude_user;
			ek = event->attr.exclude_kernel;
			eh = event->attr.exclude_hv;
			first = 0;
		} else if (event->attr.exclude_user != eu ||
			   event->attr.exclude_kernel != ek ||
			   event->attr.exclude_hv != eh) {
			return -EAGAIN;
		}
	}

	return 0;
}

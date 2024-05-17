bool cg_completion_timeout(void *fn, void *fnarg, int timeout)
{
	struct cg_completion *cgc;
	pthread_t pthread;
	bool ret = false;

	cgc = malloc(sizeof(struct cg_completion));
	if (unlikely(!cgc))
		return ret;
	cgsem_init(&cgc->cgsem);
	cgc->fn = fn;
	cgc->fnarg = fnarg;

	pthread_create(&pthread, NULL, completion_thread, (void *)cgc);

	ret = cgsem_mswait(&cgc->cgsem, timeout);
	if (!ret) {
		pthread_join(pthread, NULL);
		free(cgc);
	} else
		pthread_cancel(pthread);
	return !ret;
}

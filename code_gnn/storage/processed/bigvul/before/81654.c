event_create(void)
{
	struct posix_event *ret = mg_malloc(sizeof(struct posix_event));
	if (ret == 0) {
		 
		return 0;
	}
	if (0 != pthread_mutex_init(&(ret->mutex), NULL)) {
		 
		mg_free(ret);
		return 0;
	}
	if (0 != pthread_cond_init(&(ret->cond), NULL)) {
		 
		pthread_mutex_destroy(&(ret->mutex));
		mg_free(ret);
		return 0;
	}
	return (void *)ret;
}

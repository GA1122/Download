event_signal(void *eventhdl)
{
	struct posix_event *ev = (struct posix_event *)eventhdl;
	pthread_mutex_lock(&(ev->mutex));
	pthread_cond_signal(&(ev->cond));
	pthread_mutex_unlock(&(ev->mutex));
	return 1;
}

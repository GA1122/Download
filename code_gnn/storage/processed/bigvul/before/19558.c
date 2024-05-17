static unsigned int ep_eventpoll_poll(struct file *file, poll_table *wait)
{
	int pollflags;
	struct eventpoll *ep = file->private_data;

	 
	poll_wait(file, &ep->poll_wait, wait);

	 
	pollflags = ep_call_nested(&poll_readywalk_ncalls, EP_MAX_NESTS,
				   ep_poll_readyevents_proc, ep, ep, current);

	return pollflags != -1 ? pollflags : 0;
}

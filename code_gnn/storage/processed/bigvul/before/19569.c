static int ep_modify(struct eventpoll *ep, struct epitem *epi, struct epoll_event *event)
{
	int pwake = 0;
	unsigned int revents;
	poll_table pt;

	init_poll_funcptr(&pt, NULL);

	 
	epi->event.events = event->events;
	pt._key = event->events;
	epi->event.data = event->data;  

	 
	revents = epi->ffd.file->f_op->poll(epi->ffd.file, &pt);

	 
	if (revents & event->events) {
		spin_lock_irq(&ep->lock);
		if (!ep_is_linked(&epi->rdllink)) {
			list_add_tail(&epi->rdllink, &ep->rdllist);

			 
			if (waitqueue_active(&ep->wq))
				wake_up_locked(&ep->wq);
			if (waitqueue_active(&ep->poll_wait))
				pwake++;
		}
		spin_unlock_irq(&ep->lock);
	}

	 
	if (pwake)
		ep_poll_safewake(&ep->poll_wait);

	return 0;
}

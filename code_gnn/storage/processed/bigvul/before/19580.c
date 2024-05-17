static int ep_read_events_proc(struct eventpoll *ep, struct list_head *head,
			       void *priv)
{
	struct epitem *epi, *tmp;
	poll_table pt;

	init_poll_funcptr(&pt, NULL);
	list_for_each_entry_safe(epi, tmp, head, rdllink) {
		pt._key = epi->event.events;
		if (epi->ffd.file->f_op->poll(epi->ffd.file, &pt) &
		    epi->event.events)
			return POLLIN | POLLRDNORM;
		else {
			 
			list_del_init(&epi->rdllink);
		}
	}

	return 0;
}

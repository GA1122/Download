void eventpoll_release_file(struct file *file)
{
	struct list_head *lsthead = &file->f_ep_links;
	struct eventpoll *ep;
	struct epitem *epi;

	 
	mutex_lock(&epmutex);

	while (!list_empty(lsthead)) {
		epi = list_first_entry(lsthead, struct epitem, fllink);

		ep = epi->ep;
		list_del_init(&epi->fllink);
		mutex_lock_nested(&ep->mtx, 0);
		ep_remove(ep, epi);
		mutex_unlock(&ep->mtx);
	}

	mutex_unlock(&epmutex);
}

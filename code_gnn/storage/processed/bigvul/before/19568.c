static int ep_loop_check_proc(void *priv, void *cookie, int call_nests)
{
	int error = 0;
	struct file *file = priv;
	struct eventpoll *ep = file->private_data;
	struct eventpoll *ep_tovisit;
	struct rb_node *rbp;
	struct epitem *epi;

	mutex_lock_nested(&ep->mtx, call_nests + 1);
	ep->visited = 1;
	list_add(&ep->visited_list_link, &visited_list);
	for (rbp = rb_first(&ep->rbr); rbp; rbp = rb_next(rbp)) {
		epi = rb_entry(rbp, struct epitem, rbn);
		if (unlikely(is_file_epoll(epi->ffd.file))) {
			ep_tovisit = epi->ffd.file->private_data;
			if (ep_tovisit->visited)
				continue;
			error = ep_call_nested(&poll_loop_ncalls, EP_MAX_NESTS,
					ep_loop_check_proc, epi->ffd.file,
					ep_tovisit, current);
			if (error != 0)
				break;
		} else {
			 
			if (list_empty(&epi->ffd.file->f_tfile_llink))
				list_add(&epi->ffd.file->f_tfile_llink,
					 &tfile_check_list);
		}
	}
	mutex_unlock(&ep->mtx);

	return error;
}
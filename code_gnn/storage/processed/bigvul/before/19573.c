static int ep_poll_callback(wait_queue_t *wait, unsigned mode, int sync, void *key)
{
	int pwake = 0;
	unsigned long flags;
	struct epitem *epi = ep_item_from_wait(wait);
	struct eventpoll *ep = epi->ep;

	if ((unsigned long)key & POLLFREE) {
		ep_pwq_from_wait(wait)->whead = NULL;
		 
		list_del_init(&wait->task_list);
	}

	spin_lock_irqsave(&ep->lock, flags);

	 
	if (!(epi->event.events & ~EP_PRIVATE_BITS))
		goto out_unlock;

	 
	if (key && !((unsigned long) key & epi->event.events))
		goto out_unlock;

	 
	if (unlikely(ep->ovflist != EP_UNACTIVE_PTR)) {
		if (epi->next == EP_UNACTIVE_PTR) {
			epi->next = ep->ovflist;
			ep->ovflist = epi;
		}
		goto out_unlock;
	}

	 
	if (!ep_is_linked(&epi->rdllink))
		list_add_tail(&epi->rdllink, &ep->rdllist);

	 
	if (waitqueue_active(&ep->wq))
		wake_up_locked(&ep->wq);
	if (waitqueue_active(&ep->poll_wait))
		pwake++;

out_unlock:
	spin_unlock_irqrestore(&ep->lock, flags);

	 
	if (pwake)
		ep_poll_safewake(&ep->poll_wait);

	return 1;
}

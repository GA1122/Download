static irda_queue_t *dequeue_first(irda_queue_t **queue)
{
	irda_queue_t *ret;

	pr_debug("dequeue_first()\n");

	 
	ret =  *queue;

	if ( *queue == NULL ) {
		 
	} else if ( (*queue)->q_next == *queue ) {
		 
		*queue = NULL;
	} else {
		 
		(*queue)->q_prev->q_next = (*queue)->q_next;
		(*queue)->q_next->q_prev = (*queue)->q_prev;
		*queue = (*queue)->q_next;
	}

	 
	return ret;
}

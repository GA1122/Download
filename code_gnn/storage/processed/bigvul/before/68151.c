static irda_queue_t *dequeue_general(irda_queue_t **queue, irda_queue_t* element)
{
	irda_queue_t *ret;

	pr_debug("dequeue_general()\n");

	 
	ret =  *queue;

	if ( *queue == NULL ) {
		 
	} else if ( (*queue)->q_next == *queue ) {
		 
		*queue = NULL;

	} else {
		 
		element->q_prev->q_next = element->q_next;
		element->q_next->q_prev = element->q_prev;
		if ( (*queue) == element)
			(*queue) = element->q_next;
	}

	 
	return ret;
}

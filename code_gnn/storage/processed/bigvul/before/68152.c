static void enqueue_first(irda_queue_t **queue, irda_queue_t* element)
{

	 
	if ( *queue == NULL ) {
		 
		element->q_next = element->q_prev = *queue = element;

	} else {
		 
		element->q_next          = (*queue);
		(*queue)->q_prev->q_next = element;
		element->q_prev          = (*queue)->q_prev;
		(*queue)->q_prev         = element;
		(*queue)                 = element;
	}
}

static int snd_seq_client_enqueue_event(struct snd_seq_client *client,
					struct snd_seq_event *event,
					struct file *file, int blocking,
					int atomic, int hop)
{
	struct snd_seq_event_cell *cell;
	int err;

	 
	if (event->queue == SNDRV_SEQ_ADDRESS_SUBSCRIBERS) {
		event->dest.client = SNDRV_SEQ_ADDRESS_SUBSCRIBERS;
		event->queue = SNDRV_SEQ_QUEUE_DIRECT;
	} else
#ifdef SUPPORT_BROADCAST
		if (event->queue == SNDRV_SEQ_ADDRESS_BROADCAST) {
			event->dest.client = SNDRV_SEQ_ADDRESS_BROADCAST;
			event->queue = SNDRV_SEQ_QUEUE_DIRECT;
		}
#endif
	if (event->dest.client == SNDRV_SEQ_ADDRESS_SUBSCRIBERS) {
		 
		struct snd_seq_client_port *src_port = snd_seq_port_use_ptr(client, event->source.port);
		if (src_port == NULL)
			return -EINVAL;
		snd_seq_port_unlock(src_port);
	}

	 
	if (snd_seq_ev_is_direct(event)) {
		if (event->type == SNDRV_SEQ_EVENT_NOTE)
			return -EINVAL;  
		return snd_seq_deliver_event(client, event, atomic, hop);
	}

	 
	if (snd_seq_queue_is_used(event->queue, client->number) <= 0)
		return -EINVAL;   
	if (! snd_seq_write_pool_allocated(client))
		return -ENXIO;  

	 
	err = snd_seq_event_dup(client->pool, event, &cell, !blocking || atomic, file);
	if (err < 0)
		return err;

	 
	if ((err = snd_seq_enqueue_event(cell, atomic, hop)) < 0) {
		snd_seq_cell_free(cell);
		return err;
	}

	return 0;
}

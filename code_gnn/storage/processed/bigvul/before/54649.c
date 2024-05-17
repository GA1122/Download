void snd_seq_queue_client_leave(int client)
{
	int i;
	struct snd_seq_queue *q;

	 
	for (i = 0; i < SNDRV_SEQ_MAX_QUEUES; i++) {
		if ((q = queue_list_remove(i, client)) != NULL)
			queue_delete(q);
	}

	 
	for (i = 0; i < SNDRV_SEQ_MAX_QUEUES; i++) {
		if ((q = queueptr(i)) == NULL)
			continue;
		if (test_bit(client, q->clients_bitmap)) {
			snd_seq_prioq_leave(q->tickq, client, 0);
			snd_seq_prioq_leave(q->timeq, client, 0);
			snd_seq_queue_use(q->queue, client, 0);
		}
		queuefree(q);
	}
}

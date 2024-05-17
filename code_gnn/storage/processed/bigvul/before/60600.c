static int snd_seq_ioctl_remove_events(struct snd_seq_client *client,
				       void *arg)
{
	struct snd_seq_remove_events *info = arg;

	 
	if (info->remove_mode & SNDRV_SEQ_REMOVE_INPUT) {
		 
		if (client->type == USER_CLIENT && client->data.user.fifo)
			snd_seq_fifo_clear(client->data.user.fifo);
	}

	if (info->remove_mode & SNDRV_SEQ_REMOVE_OUTPUT)
		snd_seq_queue_remove_cells(client->number, info);

	return 0;
}

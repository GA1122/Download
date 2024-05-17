static long snd_seq_ioctl(struct file *file, unsigned int cmd,
			  unsigned long arg)
{
	struct snd_seq_client *client = file->private_data;
	 
	union {
		int pversion;
		int client_id;
		struct snd_seq_system_info	system_info;
		struct snd_seq_running_info	running_info;
		struct snd_seq_client_info	client_info;
		struct snd_seq_port_info	port_info;
		struct snd_seq_port_subscribe	port_subscribe;
		struct snd_seq_queue_info	queue_info;
		struct snd_seq_queue_status	queue_status;
		struct snd_seq_queue_tempo	tempo;
		struct snd_seq_queue_timer	queue_timer;
		struct snd_seq_queue_client	queue_client;
		struct snd_seq_client_pool	client_pool;
		struct snd_seq_remove_events	remove_events;
		struct snd_seq_query_subs	query_subs;
	} buf;
	const struct ioctl_handler *handler;
	unsigned long size;
	int err;

	if (snd_BUG_ON(!client))
		return -ENXIO;

	for (handler = ioctl_handlers; handler->cmd > 0; ++handler) {
		if (handler->cmd == cmd)
			break;
	}
	if (handler->cmd == 0)
		return -ENOTTY;

	memset(&buf, 0, sizeof(buf));

	 
	size = _IOC_SIZE(handler->cmd);
	if (handler->cmd & IOC_IN) {
		if (copy_from_user(&buf, (const void __user *)arg, size))
			return -EFAULT;
	}

	err = handler->func(client, &buf);
	if (err >= 0) {
		 
		if (handler->cmd == SNDRV_SEQ_IOCTL_SET_QUEUE_CLIENT ||
		    handler->cmd == SNDRV_SEQ_IOCTL_SET_CLIENT_POOL ||
		    (handler->cmd & IOC_OUT))
			if (copy_to_user((void __user *)arg, &buf, size))
				return -EFAULT;
	}

	return err;
}

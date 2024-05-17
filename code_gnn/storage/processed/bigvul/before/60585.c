static int snd_seq_ioctl_delete_port(struct snd_seq_client *client, void *arg)
{
	struct snd_seq_port_info *info = arg;
	int err;

	 
	if (info->addr.client != client->number)
		return -EPERM;

	err = snd_seq_delete_port(client, info->addr.port);
	if (err >= 0)
		snd_seq_system_client_ev_port_exit(client->number, info->addr.port);
	return err;
}

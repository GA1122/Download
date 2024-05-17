static int port_delete(struct snd_seq_client *client,
		       struct snd_seq_client_port *port)
{
	 
	port->closing = 1;
	snd_use_lock_sync(&port->use_lock); 

	 
	clear_subscriber_list(client, port, &port->c_src, true);
	clear_subscriber_list(client, port, &port->c_dest, false);

	if (port->private_free)
		port->private_free(port->private_data);

	snd_BUG_ON(port->c_src.count != 0);
	snd_BUG_ON(port->c_dest.count != 0);

	kfree(port);
	return 0;
}

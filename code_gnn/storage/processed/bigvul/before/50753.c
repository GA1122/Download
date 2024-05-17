static int network_receive (void)  
{
	char buffer[network_config_packet_size];
	int  buffer_len;

	int i;
	int status = 0;

	receive_list_entry_t *private_list_head;
	receive_list_entry_t *private_list_tail;
	uint64_t              private_list_length;

	assert (listen_sockets_num > 0);

	private_list_head = NULL;
	private_list_tail = NULL;
	private_list_length = 0;

	while (listen_loop == 0)
	{
		status = poll (listen_sockets_pollfd, listen_sockets_num, -1);
		if (status <= 0)
		{
			char errbuf[1024];
			if (errno == EINTR)
				continue;
			ERROR ("network plugin: poll(2) failed: %s",
					sstrerror (errno, errbuf, sizeof (errbuf)));
			break;
		}

		for (i = 0; (i < listen_sockets_num) && (status > 0); i++)
		{
			receive_list_entry_t *ent;

			if ((listen_sockets_pollfd[i].revents
						& (POLLIN | POLLPRI)) == 0)
				continue;
			status--;

			buffer_len = recv (listen_sockets_pollfd[i].fd,
					buffer, sizeof (buffer),
					0  );
			if (buffer_len < 0)
			{
				char errbuf[1024];
				status = (errno != 0) ? errno : -1;
				ERROR ("network plugin: recv(2) failed: %s",
						sstrerror (errno, errbuf, sizeof (errbuf)));
				break;
			}

			stats_octets_rx += ((uint64_t) buffer_len);
			stats_packets_rx++;

			 
			ent = malloc (sizeof (receive_list_entry_t));
			if (ent == NULL)
			{
				ERROR ("network plugin: malloc failed.");
				status = ENOMEM;
				break;
			}
			memset (ent, 0, sizeof (receive_list_entry_t));
			ent->data = malloc (network_config_packet_size);
			if (ent->data == NULL)
			{
				sfree (ent);
				ERROR ("network plugin: malloc failed.");
				status = ENOMEM;
				break;
			}
			ent->fd = listen_sockets_pollfd[i].fd;
			ent->next = NULL;

			memcpy (ent->data, buffer, buffer_len);
			ent->data_len = buffer_len;

			if (private_list_head == NULL)
				private_list_head = ent;
			else
				private_list_tail->next = ent;
			private_list_tail = ent;
			private_list_length++;

			 
			if (pthread_mutex_trylock (&receive_list_lock) == 0)
			{
				assert (((receive_list_head == NULL) && (receive_list_length == 0))
						|| ((receive_list_head != NULL) && (receive_list_length != 0)));

				if (receive_list_head == NULL)
					receive_list_head = private_list_head;
				else
					receive_list_tail->next = private_list_head;
				receive_list_tail = private_list_tail;
				receive_list_length += private_list_length;

				pthread_cond_signal (&receive_list_cond);
				pthread_mutex_unlock (&receive_list_lock);

				private_list_head = NULL;
				private_list_tail = NULL;
				private_list_length = 0;
			}

			status = 0;
		}  

		if (status != 0)
			break;
	}  

	 
	if (private_list_head != NULL)
	{
		pthread_mutex_lock (&receive_list_lock);

		if (receive_list_head == NULL)
			receive_list_head = private_list_head;
		else
			receive_list_tail->next = private_list_head;
		receive_list_tail = private_list_tail;
		receive_list_length += private_list_length;

		pthread_cond_signal (&receive_list_cond);
		pthread_mutex_unlock (&receive_list_lock);
	}

	return (status);
}  
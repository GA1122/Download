static int network_shutdown (void)
{
	sockent_t *se;

	listen_loop++;

	 
	if (receive_thread_running != 0)
	{
		INFO ("network plugin: Stopping receive thread.");
		pthread_kill (receive_thread_id, SIGTERM);
		pthread_join (receive_thread_id, NULL  );
		memset (&receive_thread_id, 0, sizeof (receive_thread_id));
		receive_thread_running = 0;
	}

	 
	if (dispatch_thread_running != 0)
	{
		INFO ("network plugin: Stopping dispatch thread.");
		pthread_mutex_lock (&receive_list_lock);
		pthread_cond_broadcast (&receive_list_cond);
		pthread_mutex_unlock (&receive_list_lock);
		pthread_join (dispatch_thread_id,   NULL);
		dispatch_thread_running = 0;
	}

	sockent_destroy (listen_sockets);

	if (send_buffer_fill > 0)
		flush_buffer ();

	sfree (send_buffer);

	for (se = sending_sockets; se != NULL; se = se->next)
		sockent_client_disconnect (se);
	sockent_destroy (sending_sockets);

	plugin_unregister_config ("network");
	plugin_unregister_init ("network");
	plugin_unregister_write ("network");
	plugin_unregister_shutdown ("network");

	return (0);
}  

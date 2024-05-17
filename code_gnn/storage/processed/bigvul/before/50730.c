static void *dispatch_thread (void __attribute__((unused)) *arg)  
{
  while (42)
  {
    receive_list_entry_t *ent;
    sockent_t *se;

     
    pthread_mutex_lock (&receive_list_lock);
    while ((listen_loop == 0)
        && (receive_list_head == NULL))
      pthread_cond_wait (&receive_list_cond, &receive_list_lock);

     
    ent = receive_list_head;
    if (ent != NULL)
      receive_list_head = ent->next;
    receive_list_length--;
    pthread_mutex_unlock (&receive_list_lock);

     
    if (ent == NULL)
      break;

     
    se = listen_sockets;
    while (se != NULL)
    {
      size_t i;

      for (i = 0; i < se->data.server.fd_num; i++)
        if (se->data.server.fd[i] == ent->fd)
          break;

      if (i < se->data.server.fd_num)
        break;

      se = se->next;
    }

    if (se == NULL)
    {
      ERROR ("network plugin: Got packet from FD %i, but can't "
          "find an appropriate socket entry.",
          ent->fd);
      sfree (ent->data);
      sfree (ent);
      continue;
    }

    parse_packet (se, ent->data, ent->data_len,   0,
	  NULL);
    sfree (ent->data);
    sfree (ent);
  }  

  return (NULL);
}  
update_socket_messages (ProxySide *side, Buffer *buffer, Header *header)
{
   

  side->control_messages = g_list_concat (side->control_messages, buffer->control_messages);
  buffer->control_messages = NULL;
  if (header->unix_fds > 0)
    {
      buffer->control_messages = side_get_n_unix_fds (side, header->unix_fds);
      if (buffer->control_messages == NULL)
        {
          g_warning ("Not enough fds for message");
          side_closed (side);
          buffer_unref (buffer);
          return FALSE;
        }
    }
  return TRUE;
}

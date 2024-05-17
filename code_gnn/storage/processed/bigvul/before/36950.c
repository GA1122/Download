_fep_transceive_control_message (Fep               *fep,
                                 int                fd,
                                 FepControlMessage *request,
                                 FepControlMessage *response)
{
  FepList *messages = NULL;
  int retval = 0;

  retval = _fep_write_control_message (fd, request);
  if (retval < 0)
    return retval;

  while (true)
    {
      FepControlMessage message;

      retval = _fep_read_control_message (fd, &message);
      if (retval < 0)
	goto out;

      if (message.command == FEP_CONTROL_RESPONSE)
	{
	  memcpy (response, &message, sizeof (FepControlMessage));
	  break;
	}

      fep_log (FEP_LOG_LEVEL_DEBUG,
	       "not a control response %d",
	       message.command);

      messages = _fep_append_control_message (messages, &message);
    }

  if (response->n_args == 0)
    {
      _fep_control_message_free_args (response);
      fep_log (FEP_LOG_LEVEL_WARNING,
	       "too few arguments for RESPONSE");
      retval = -1;
      goto out;
    }

  if (response->args[0].len != 1)
    {
      _fep_control_message_free_args (response);
      fep_log (FEP_LOG_LEVEL_WARNING,
	       "can't extract command from RESPONSE");
      retval = -1;
      goto out;
    }

  if (*response->args[0].str != request->command)
    {
      _fep_control_message_free_args (response);
      fep_log (FEP_LOG_LEVEL_WARNING,
	       "commands do not match (%d != %d)",
	       *response->args[0].str,
	       request->command);
      retval = -1;
      goto out;
    }

 out:
   
  while (messages)
    {
      FepList *_head = messages;
      FepControlMessage *_message = _head->data;

      messages = _head->next;

      _fep_dispatch_control_message (fep, _message);
      _fep_control_message_free (_message);
      free (_head);
    }
  return retval;
}
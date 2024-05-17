uiserver_set_fd (engine_uiserver_t uiserver, fd_type_t fd_type, const char *opt)
{
  gpg_error_t err = 0;
  char line[COMMANDLINELEN];
  char *which;
  iocb_data_t *iocb_data;
  int dir;

  switch (fd_type)
    {
    case INPUT_FD:
      which = "INPUT";
      iocb_data = &uiserver->input_cb;
      break;

    case OUTPUT_FD:
      which = "OUTPUT";
      iocb_data = &uiserver->output_cb;
      break;

    case MESSAGE_FD:
      which = "MESSAGE";
      iocb_data = &uiserver->message_cb;
      break;

    default:
      return gpg_error (GPG_ERR_INV_VALUE);
    }

  dir = iocb_data->dir;

   
  iocb_data->server_fd = _gpgme_data_get_fd (iocb_data->data);
  if (iocb_data->server_fd < 0)
    {
      int fds[2];

      if (_gpgme_io_pipe (fds, 0) < 0)
	return gpg_error_from_syserror ();

      iocb_data->fd = dir ? fds[0] : fds[1];
      iocb_data->server_fd = dir ? fds[1] : fds[0];

      if (_gpgme_io_set_close_notify (iocb_data->fd,
				      close_notify_handler, uiserver))
	{
	  err = gpg_error (GPG_ERR_GENERAL);
	  goto leave_set_fd;
	}
    }

  err = assuan_sendfd (uiserver->assuan_ctx, iocb_data->server_fd);
  if (err)
    goto leave_set_fd;

  _gpgme_io_close (iocb_data->server_fd);
  iocb_data->server_fd = -1;

  if (opt)
    snprintf (line, COMMANDLINELEN, "%s FD %s", which, opt);
  else
    snprintf (line, COMMANDLINELEN, "%s FD", which);

  err = uiserver_assuan_simple_command (uiserver->assuan_ctx, line, NULL, NULL);

 leave_set_fd:
  if (err)
    {
      _gpgme_io_close (iocb_data->fd);
      iocb_data->fd = -1;
      if (iocb_data->server_fd != -1)
        {
          _gpgme_io_close (iocb_data->server_fd);
          iocb_data->server_fd = -1;
        }
    }

  return err;
}

uiserver_cancel (void *engine)
{
  engine_uiserver_t uiserver = engine;

  if (!uiserver)
    return gpg_error (GPG_ERR_INV_VALUE);

  if (uiserver->status_cb.fd != -1)
    _gpgme_io_close (uiserver->status_cb.fd);
  if (uiserver->input_cb.fd != -1)
    _gpgme_io_close (uiserver->input_cb.fd);
  if (uiserver->output_cb.fd != -1)
    _gpgme_io_close (uiserver->output_cb.fd);
  if (uiserver->message_cb.fd != -1)
    _gpgme_io_close (uiserver->message_cb.fd);

  if (uiserver->assuan_ctx)
    {
      assuan_release (uiserver->assuan_ctx);
      uiserver->assuan_ctx = NULL;
    }

  return 0;
}

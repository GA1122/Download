luks_lock_completed_cb (DBusGMethodInvocation *context,
                        Device *device,
                        gboolean job_was_cancelled,
                        int status,
                        const char *stderr,
                        const char *stdout,
                        gpointer user_data)
{
  LockEncryptionData *data = user_data;

  if (WEXITSTATUS (status) == 0 && !job_was_cancelled)
    {

       
      if (data->cleartext_device->priv->removed)
        {
           
          update_info (data->luks_device);
          drain_pending_changes (data->luks_device, FALSE);

          dbus_g_method_return (context);
        }
      else
        {
           

          data->device_removed_signal_handler_id
            = g_signal_connect_after (device->priv->daemon,
                                      "device-removed",
                                      (GCallback) luks_lock_wait_for_cleartext_device_removed_cb,
                                      lock_encryption_data_ref (data));

           
          data->device_removed_timeout_id = g_timeout_add (10 * 1000,
                                                           luks_lock_wait_for_cleartext_device_not_seen_cb,
                                                           data);

          job_local_start (device, "LuksLock");
        }
    }
  else
    {
      if (job_was_cancelled)
        {
          throw_error (context, ERROR_CANCELLED, "Job was cancelled");
        }
      else
        {
          throw_error (context,
                       ERROR_FAILED,
                       "Error locking device: cryptsetup exited with exit code %d: %s",
                       WEXITSTATUS (status),
                       stderr);
        }
    }
}
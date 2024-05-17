device_luks_unlock_internal (Device *device,
                             const char *secret,
                             char **options,
                             UnlockEncryptionHookFunc hook_func,
                             gpointer hook_user_data,
                             DBusGMethodInvocation *context)
{
  int n;
  char *argv[10];
  char *luks_name;
  GError *error;
  char *secret_as_stdin;
  uid_t uid;

  luks_name = NULL;
  secret_as_stdin = NULL;
  error = NULL;

  daemon_local_get_uid (device->priv->daemon, &uid, context);

  if (device_local_is_busy (device, FALSE, &error))
    {
      dbus_g_method_return_error (context, error);
      g_error_free (error);
      goto out;
    }

  if (device->priv->id_usage == NULL || strcmp (device->priv->id_usage, "crypto") != 0)
    {
      throw_error (context, ERROR_FAILED, "Not a LUKS device");
      goto out;
    }

  if (find_cleartext_device (device) != NULL)
    {
      throw_error (context, ERROR_FAILED, "Cleartext device is already unlocked");
      goto out;
    }

  luks_name = g_strdup_printf ("udisks-luks-uuid-%s-uid%d", device->priv->id_uuid, uid);
  secret_as_stdin = g_strdup_printf ("%s\n", secret);

  n = 0;
  argv[n++] = "cryptsetup";
  argv[n++] = "-T";
  argv[n++] = "1";
  argv[n++] = "luksOpen";
  argv[n++] = device->priv->device_file;
  argv[n++] = luks_name;
  argv[n++] = NULL;

   
  if (!job_new (context,
                "LuksUnlock",
                FALSE,
                device,
                argv,
                secret_as_stdin,
                luks_unlock_completed_cb,
                TRUE,  
                unlock_encryption_data_new (context, device, hook_func, hook_user_data),
                (GDestroyNotify) unlock_encryption_data_unref))
    {
      goto out;
    }

 out:
   
  if (secret_as_stdin != NULL)
    {
      memset (secret_as_stdin, '\0', strlen (secret_as_stdin));
    }
  g_free (secret_as_stdin);
  g_free (luks_name);
  return TRUE;
}
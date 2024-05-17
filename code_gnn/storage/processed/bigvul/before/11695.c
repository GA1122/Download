filesystem_create_wait_for_cleartext_device_hook (DBusGMethodInvocation *context,
                                                  Device *device,
                                                  gpointer user_data)
{
  MkfsLuksData *data = user_data;

  if (device == NULL)
    {
       
    }
  else
    {
       

      device_filesystem_create_internal (device,
                                         data->fstype,
                                         data->options,
                                         data->mkfs_hook_func,
                                         data->mkfs_hook_user_data,
                                         data->context);
      mkfse_data_unref (data);
    }
}

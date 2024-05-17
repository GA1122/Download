partition_create_device_added_cb (Daemon *daemon,
                                  const char *object_path,
                                  gpointer user_data)
{
  CreatePartitionData *data = user_data;
  Device *device;

   
  device = daemon_local_find_by_object_path (daemon, object_path);
  if (device != NULL && device->priv->device_is_partition && strcmp (device->priv->partition_slave,
                                                                     data->device->priv->object_path) == 0
      && data->created_offset == device->priv->partition_offset && data->created_size == device->priv->partition_size)
    {

       
      partition_create_found_device (device, data);

      g_signal_handler_disconnect (daemon, data->device_added_signal_handler_id);
      g_source_remove (data->device_added_timeout_id);
      partition_create_data_unref (data);
    }
}

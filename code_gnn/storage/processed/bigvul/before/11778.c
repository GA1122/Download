lvm2_lv_create_device_changed_cb (Daemon *daemon,
                                  const char *object_path,
                                  gpointer user_data)
{
  CreateLvm2LVData *data = user_data;
  Device *device;

  g_debug ("changed %s", object_path);

  device = lvm2_lv_create_has_lv (data);
  if (device != NULL)
    {
       
      lvm2_lv_create_found_device (device, data);

      g_signal_handler_disconnect (daemon, data->device_added_signal_handler_id);
      g_signal_handler_disconnect (daemon, data->device_changed_signal_handler_id);
      g_source_remove (data->device_added_timeout_id);
      lvm2_lv_create_data_unref (data);
    }
}
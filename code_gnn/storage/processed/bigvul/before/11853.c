update_info_partition_table (Device *device)
{
  gboolean is_partition_table;

  is_partition_table = FALSE;

   
  if (g_udev_device_get_property_as_boolean (device->priv->d, "UDISKS_PARTITION_TABLE"))
    {
      device_set_partition_table_scheme (device,
                                         g_udev_device_get_property (device->priv->d,
                                                                     "UDISKS_PARTITION_TABLE_SCHEME"));
      device_set_partition_table_count (device,
                                        g_udev_device_get_property_as_int (device->priv->d,
                                                                           "UDISKS_PARTITION_TABLE_COUNT"));
      is_partition_table = TRUE;
    }

   
  if (!is_partition_table)
    {
      gchar *s;
      GDir *dir;

      s = g_path_get_basename (device->priv->native_path);
      if ((dir = g_dir_open (device->priv->native_path, 0, NULL)) != NULL)
        {
          guint partition_count;
          const gchar *name;

          partition_count = 0;
          while ((name = g_dir_read_name (dir)) != NULL)
            {
              if (g_str_has_prefix (name, s))
                {
                  partition_count++;
                }
            }
          g_dir_close (dir);

          if (partition_count > 0)
            {
              device_set_partition_table_scheme (device, "");
              device_set_partition_table_count (device, partition_count);
              is_partition_table = TRUE;
            }
        }
      g_free (s);
    }

  device_set_device_is_partition_table (device, is_partition_table);
  if (!is_partition_table)
    {
       
      device_set_partition_table_scheme (device, NULL);
      device_set_partition_table_count (device, 0);
    }

  return TRUE;
}

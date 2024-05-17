update_info_is_system_internal (Device *device)
{
  gboolean is_system_internal;

   

   
  is_system_internal = TRUE;

   
  if (device->priv->device_is_linux_md)
    {
      is_system_internal = FALSE;

      if (device->priv->slaves_objpath->len == 0)
        {
          is_system_internal = TRUE;
        }
      else
        {
          guint n;

          for (n = 0; n < device->priv->slaves_objpath->len; n++)
            {
              const gchar *slave_objpath;
              Device *slave;

              slave_objpath = device->priv->slaves_objpath->pdata[n];
              slave = daemon_local_find_by_object_path (device->priv->daemon, slave_objpath);
              if (slave == NULL)
                continue;

              if (slave->priv->device_is_system_internal)
                {
                  is_system_internal = TRUE;
                  break;
                }
            }
        }

      goto determined;
    }

   
  if (device->priv->device_is_partition)
    {
      Device *enclosing_device;

      enclosing_device = daemon_local_find_by_object_path (device->priv->daemon, device->priv->partition_slave);
      if (enclosing_device != NULL)
        {
          is_system_internal = enclosing_device->priv->device_is_system_internal;
        }
      else
        {
          is_system_internal = TRUE;
        }

      goto determined;
    }

   
  if (device->priv->device_is_luks_cleartext)
    {
      Device *enclosing_device;
      enclosing_device = daemon_local_find_by_object_path (device->priv->daemon, device->priv->luks_cleartext_slave);
      if (enclosing_device != NULL)
        {
          is_system_internal = enclosing_device->priv->device_is_system_internal;
        }
      else
        {
          is_system_internal = TRUE;
        }

      goto determined;
    }

   
  if (device->priv->device_is_removable)
    {
      is_system_internal = FALSE;
      goto determined;
    }

   
  if (device->priv->device_is_drive && device->priv->drive_connection_interface != NULL)
    {

      if (strcmp (device->priv->drive_connection_interface, "ata_serial_esata") == 0
          || strcmp (device->priv->drive_connection_interface, "sdio") == 0
          || strcmp (device->priv->drive_connection_interface, "usb") == 0
          || strcmp (device->priv->drive_connection_interface, "firewire") == 0)
        {
          is_system_internal = FALSE;
        }
      else
        {
          is_system_internal = TRUE;
        }
      goto determined;
    }

 determined:
  device_set_device_is_system_internal (device, is_system_internal);

  return TRUE;
}

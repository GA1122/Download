update_info_media_detection (Device *device)
{
  gboolean detected;
  gboolean polling;
  gboolean inhibitable;
  gboolean inhibited;

  detected = FALSE;
  polling = FALSE;
  inhibitable = FALSE;
  inhibited = FALSE;

  if (device->priv->device_is_removable)
    {
      guint64 evt_media_change;
      GUdevDevice *parent;

      evt_media_change = sysfs_get_uint64 (device->priv->native_path, "../../evt_media_change");
      if (evt_media_change & 1)
        {
           

          polling = FALSE;
          detected = TRUE;
          goto determined;
        }

      parent = g_udev_device_get_parent_with_subsystem (device->priv->d, "platform", NULL);
      if (parent != NULL)
        {
           
          if (g_str_has_prefix (g_udev_device_get_name (parent), "floppy."))
            {
              g_object_unref (parent);
              goto determined;
            }
          g_object_unref (parent);
        }

       
      polling = TRUE;
      inhibitable = TRUE;

       
      if (g_udev_device_has_property (device->priv->d, "UDISKS_DISABLE_POLLING") &&
	  g_udev_device_get_property_as_boolean (device->priv->d, "UDISKS_DISABLE_POLLING"))
	polling = FALSE;

      if (device->priv->polling_inhibitors != NULL || daemon_local_has_polling_inhibitors (device->priv->daemon))
        {

          detected = FALSE;
          inhibited = TRUE;
        }
      else
        {
          detected = TRUE;
          inhibited = FALSE;
        }
    }

 determined:
  device_set_device_is_media_change_detected (device, detected);
  device_set_device_is_media_change_detection_polling (device, polling);
  device_set_device_is_media_change_detection_inhibitable (device, inhibitable);
  device_set_device_is_media_change_detection_inhibited (device, inhibited);

  return TRUE;
}

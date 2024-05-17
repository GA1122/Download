update_info_drive (Device *device)
{
  GPtrArray *media_compat_array;
  const gchar *media_in_drive;
  gboolean drive_is_ejectable;
  gboolean drive_can_detach;
  gchar *decoded_string;
  guint n;

  if (g_udev_device_has_property (device->priv->d, "ID_VENDOR_ENC"))
    {
      decoded_string = decode_udev_encoded_string (g_udev_device_get_property (device->priv->d, "ID_VENDOR_ENC"));
      g_strstrip (decoded_string);
      device_set_drive_vendor (device, decoded_string);
      g_free (decoded_string);
    }
  else if (g_udev_device_has_property (device->priv->d, "ID_VENDOR"))
    {
      device_set_drive_vendor (device, g_udev_device_get_property (device->priv->d, "ID_VENDOR"));
    }

  if (g_udev_device_has_property (device->priv->d, "ID_MODEL_ENC"))
    {
      decoded_string = decode_udev_encoded_string (g_udev_device_get_property (device->priv->d, "ID_MODEL_ENC"));
      g_strstrip (decoded_string);
      device_set_drive_model (device, decoded_string);
      g_free (decoded_string);
    }
  else if (g_udev_device_has_property (device->priv->d, "ID_MODEL"))
    {
      device_set_drive_model (device, g_udev_device_get_property (device->priv->d, "ID_MODEL"));
    }

  if (g_udev_device_has_property (device->priv->d, "ID_REVISION"))
    device_set_drive_revision (device, g_udev_device_get_property (device->priv->d, "ID_REVISION"));
  if (g_udev_device_has_property (device->priv->d, "ID_SCSI_SERIAL"))
    {
       
      device_set_drive_serial (device, g_udev_device_get_property (device->priv->d, "ID_SCSI_SERIAL"));
    }
  else if (g_udev_device_has_property (device->priv->d, "ID_SERIAL_SHORT"))
    {
      device_set_drive_serial (device, g_udev_device_get_property (device->priv->d, "ID_SERIAL_SHORT"));
    }

  if (g_udev_device_has_property (device->priv->d, "ID_WWN_WITH_EXTENSION"))
    device_set_drive_wwn (device, g_udev_device_get_property (device->priv->d, "ID_WWN_WITH_EXTENSION") + 2);
  else if (g_udev_device_has_property (device->priv->d, "ID_WWN"))
    device_set_drive_wwn (device, g_udev_device_get_property (device->priv->d, "ID_WWN") + 2);

   
  update_drive_properties_from_sysfs (device);

  if (g_udev_device_has_property (device->priv->d, "ID_DRIVE_EJECTABLE"))
    {
      drive_is_ejectable = g_udev_device_get_property_as_boolean (device->priv->d, "ID_DRIVE_EJECTABLE");
    }
  else
    {
      drive_is_ejectable = FALSE;
      drive_is_ejectable |= g_udev_device_has_property (device->priv->d, "ID_CDROM");
      drive_is_ejectable |= g_udev_device_has_property (device->priv->d, "ID_DRIVE_FLOPPY_ZIP");
      drive_is_ejectable |= g_udev_device_has_property (device->priv->d, "ID_DRIVE_FLOPPY_JAZ");
    }
  device_set_drive_is_media_ejectable (device, drive_is_ejectable);

  media_compat_array = g_ptr_array_new ();
  for (n = 0; drive_media_mapping[n].udev_property != NULL; n++)
    {
      if (!g_udev_device_has_property (device->priv->d, drive_media_mapping[n].udev_property))
        continue;

      g_ptr_array_add (media_compat_array, (gpointer) drive_media_mapping[n].media_name);
    }
   
  if (g_strcmp0 (device->priv->drive_connection_interface, "sdio") == 0)
    {
      gchar *type;

      type = sysfs_get_string (device->priv->native_path, "../../type");
      g_strstrip (type);
      if (g_strcmp0 (type, "MMC") == 0)
        {
          g_ptr_array_add (media_compat_array, "flash_mmc");
        }
      else if (g_strcmp0 (type, "SD") == 0)
        {
          g_ptr_array_add (media_compat_array, "flash_sd");
        }
      else if (g_strcmp0 (type, "SDHC") == 0)
        {
          g_ptr_array_add (media_compat_array, "flash_sdhc");
        }
      g_free (type);
    }
  g_ptr_array_sort (media_compat_array, (GCompareFunc) ptr_str_array_compare);
  g_ptr_array_add (media_compat_array, NULL);
  device_set_drive_media_compatibility (device, (GStrv) media_compat_array->pdata);

  media_in_drive = NULL;

  if (device->priv->device_is_media_available)
    {
      for (n = 0; media_mapping[n].udev_property != NULL; n++)
        {
          if (!g_udev_device_has_property (device->priv->d, media_mapping[n].udev_property))
            continue;

          media_in_drive = drive_media_mapping[n].media_name;
          break;
        }
       
      if (media_in_drive == NULL)
        media_in_drive = ((const gchar **) media_compat_array->pdata)[0];
    }
  device_set_drive_media (device, media_in_drive);

  g_ptr_array_free (media_compat_array, TRUE);

   
  drive_can_detach = FALSE;
  if (g_strcmp0 (device->priv->drive_connection_interface, "usb") == 0)
    {
      drive_can_detach = TRUE;
    }
  if (g_udev_device_has_property (device->priv->d, "ID_DRIVE_DETACHABLE"))
    {
      drive_can_detach = g_udev_device_get_property_as_boolean (device->priv->d, "ID_DRIVE_DETACHABLE");
    }
  device_set_drive_can_detach (device, drive_can_detach);

   
  device_set_drive_is_rotational (device, g_udev_device_get_sysfs_attr_as_boolean (device->priv->d, "queue/rotational"));

  if (g_udev_device_has_property (device->priv->d, "ID_ATA_ROTATION_RATE_RPM"))
    {
      device_set_drive_rotation_rate (device, g_udev_device_get_property_as_int (device->priv->d,
                                                                                 "ID_ATA_ROTATION_RATE_RPM"));
    }

  if (g_udev_device_get_property_as_boolean (device->priv->d, "ID_ATA_WRITE_CACHE"))
    {
      if (g_udev_device_get_property_as_boolean (device->priv->d, "ID_ATA_WRITE_CACHE_ENABLED"))
        {
          device_set_drive_write_cache (device, "enabled");
        }
      else
        {
          device_set_drive_write_cache (device, "disabled");
        }
    }

  return TRUE;
}

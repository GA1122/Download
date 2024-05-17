update_info_drive_ata_smart (Device *device)
{
  gboolean ata_smart_is_available;

  ata_smart_is_available = FALSE;
  if (device->priv->device_is_drive && g_udev_device_has_property (device->priv->d, "UDISKS_ATA_SMART_IS_AVAILABLE"))
    ata_smart_is_available = g_udev_device_get_property_as_boolean (device->priv->d, "UDISKS_ATA_SMART_IS_AVAILABLE");

  device_set_drive_ata_smart_is_available (device, ata_smart_is_available);

   

  return TRUE;
}

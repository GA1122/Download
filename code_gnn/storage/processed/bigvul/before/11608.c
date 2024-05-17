device_changed (Device *device,
                GUdevDevice *d,
                gboolean synthesized)
{
  gboolean keep_device;

  g_object_unref (device->priv->d);
  device->priv->d = g_object_ref (d);

  keep_device = update_info (device);

   
  if (!keep_device)
    goto out;

   
  drain_pending_changes (device, !synthesized);

   
  if (!device->priv->device_is_media_available)
    {
      GList *l;
      GList *devices;

      force_removal (device, NULL, NULL);

       
      devices = daemon_local_get_all_devices (device->priv->daemon);
      for (l = devices; l != NULL; l = l->next)
        {
          Device *d = DEVICE (l->data);

          if (d->priv->device_is_partition && d->priv->partition_slave != NULL && strcmp (d->priv->partition_slave,
                                                                                          device->priv->object_path)
              == 0)
            {

              force_removal (d, NULL, NULL);
            }
        }

      g_list_free (devices);
    }
 out:
  return keep_device;
}

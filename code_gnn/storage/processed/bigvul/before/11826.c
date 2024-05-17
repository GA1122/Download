update_drive_properties_from_sysfs (Device *device)
{
  char *s;
  char *p;
  char *q;
  char *model;
  char *vendor;
  char *subsystem;
  char *serial;
  char *revision;
  const char *connection_interface;
  guint64 connection_speed;

  connection_interface = NULL;
  connection_speed = 0;

   
  s = g_strdup (device->priv->native_path);
  do
    {
      p = sysfs_resolve_link (s, "subsystem");
      if (p != NULL)
        {
          subsystem = g_path_get_basename (p);
          g_free (p);

          if (strcmp (subsystem, "scsi") == 0)
            {
              connection_interface = "scsi";
              connection_speed = 0;

               

               
              vendor = sysfs_get_string (s, "vendor");
              if (vendor != NULL)
                {
                  g_strstrip (vendor);
                   
                  if (device->priv->drive_vendor == NULL)
                    {
                      q = _dupv8 (vendor);
                      device_set_drive_vendor (device, q);
                      g_free (q);
                    }
                  g_free (vendor);
                }

              model = sysfs_get_string (s, "model");
              if (model != NULL)
                {
                  g_strstrip (model);
                   
                  if (device->priv->drive_model == NULL)
                    {
                      q = _dupv8 (model);
                      device_set_drive_model (device, q);
                      g_free (q);
                    }
                  g_free (model);
                }

               

              if (device->priv->drive_vendor != NULL && strcmp (device->priv->drive_vendor, "ATA") == 0)
                {
                  connection_interface = "ata";
                  break;
                }

            }
          else if (strcmp (subsystem, "usb") == 0)
            {
              double usb_speed;

               
              usb_speed = sysfs_get_double (s, "speed");
              if (usb_speed > 0)
                {
                  connection_interface = "usb";
                  connection_speed = usb_speed * (1000 * 1000);
                  break;

                }
            }
          else if (strcmp (subsystem, "firewire") == 0 || strcmp (subsystem, "ieee1394") == 0)
            {

               

              connection_interface = "firewire";
              connection_speed = 400 * (1000 * 1000);
              break;

            }
          else if (strcmp (subsystem, "mmc") == 0)
            {

               
              connection_interface = "sdio";

               

              model = sysfs_get_string (s, "name");
              if (model != NULL)
                {
                  g_strstrip (model);
                   
                  if (device->priv->drive_model == NULL)
                    {
                      q = _dupv8 (model);
                      device_set_drive_model (device, q);
                      g_free (q);
                    }
                  g_free (model);
                }

              serial = sysfs_get_string (s, "serial");
              if (serial != NULL)
                {
                  g_strstrip (serial);
                   
                  if (device->priv->drive_serial == NULL)
                    {
                       
                      q = _dupv8 (serial + 2);
                      device_set_drive_serial (device, q);
                      g_free (q);
                    }
                  g_free (serial);
                }

               
              revision = sysfs_get_string (s, "date");
              if (revision != NULL)
                {
                  g_strstrip (revision);
                   
                  if (device->priv->drive_revision == NULL)
                    {
                      q = _dupv8 (revision);
                      device_set_drive_revision (device, q);
                      g_free (q);
                    }
                  g_free (revision);
                }

               

            }
          else if (strcmp (subsystem, "platform") == 0)
            {
              const gchar *sysfs_name;

              sysfs_name = g_strrstr (s, "/");
              if (g_str_has_prefix (sysfs_name + 1, "floppy."))
                {
                  device_set_drive_vendor (device, "Floppy Drive");
                  connection_interface = "platform";
                }
            }

          g_free (subsystem);
        }

       
      p = g_strrstr (s, "/");
      if (p == NULL)
        break;
      *p = '\0';

       
      if (strcmp (s, "/sys/devices") == 0)
        break;

    }
  while (TRUE);

  if (connection_interface != NULL)
    {
      device_set_drive_connection_interface (device, connection_interface);
      device_set_drive_connection_speed (device, connection_speed);
    }

  g_free (s);
}

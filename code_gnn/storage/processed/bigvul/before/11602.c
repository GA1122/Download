daemon_linux_lvm2_vg_stop_authorized_cb (Daemon *daemon,
                                         Device *device,
                                         DBusGMethodInvocation *context,
                                         const gchar *action_id,
                                         guint num_user_data,
                                         gpointer *user_data_elements)
{
  const gchar *uuid = user_data_elements[0];
  const gchar *vg_name;
   
  guint n;
  gchar *argv[10];

   
  vg_name = find_lvm2_vg_name_for_uuid (daemon, uuid);
  if (vg_name == NULL)
    {
      throw_error (context, ERROR_FAILED, "Cannot find VG with UUID `%s'", uuid);
      goto out;
    }

  n = 0;
  argv[n++] = "vgchange";
  argv[n++] = "-an";
  argv[n++] = (gchar *) vg_name;
  argv[n++] = NULL;

  if (!job_new (context, "LinuxLvm2VGStop", TRUE, NULL, argv, NULL, linux_lvm2_vg_stop_completed_cb, FALSE, NULL, NULL))
    {
      goto out;
    }

 out:
  ;
}
update_info_in_idle_cb (gpointer user_data)
{
  UpdateInfoInIdleData *data = user_data;

   
  daemon_local_synthesize_changed (data->device->priv->daemon, data->device);

  return FALSE;  
}

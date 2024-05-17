drain_pending_changes (Device *device,
                       gboolean force_update)
{
  gboolean emit_changed;

  emit_changed = FALSE;

   
  if (device->priv->emit_changed_idle_id != 0)
    {
      g_source_remove (device->priv->emit_changed_idle_id);
      device->priv->emit_changed_idle_id = 0;
      emit_changed = TRUE;
    }

  if ((!device->priv->removed) && (emit_changed || force_update))
    {
      if (device->priv->object_path != NULL)
        {
          g_print ("**** EMITTING CHANGED for %s\n", device->priv->native_path);
          g_signal_emit_by_name (device, "changed");
          g_signal_emit_by_name (device->priv->daemon, "device-changed", device->priv->object_path);
        }
    }
}

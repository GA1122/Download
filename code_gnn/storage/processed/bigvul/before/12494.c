bus_activation_entry_unref (BusActivationEntry *entry)
{
  if (entry == NULL)  
    return;

  _dbus_assert (entry->refcount > 0);
  entry->refcount--;

  if (entry->refcount > 0)
    return;

  dbus_free (entry->name);
  dbus_free (entry->exec);
  dbus_free (entry->user);
  dbus_free (entry->filename);
  dbus_free (entry->systemd_service);

  dbus_free (entry);
}

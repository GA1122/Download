bus_activation_entry_ref (BusActivationEntry *entry)
{
  _dbus_assert (entry->refcount > 0);
  entry->refcount++;

  return entry;
}

bus_owner_unref  (BusOwner *owner)
{
  _dbus_assert (owner->refcount > 0);
  owner->refcount -= 1;

  if (owner->refcount == 0)
    {
      bus_connection_remove_owned_service (owner->conn, owner->service);
      _dbus_mem_pool_dealloc (owner->service->registry->owner_pool, owner);
    }
}

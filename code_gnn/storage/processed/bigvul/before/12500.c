bus_owner_ref (BusOwner *owner)
{
  _dbus_assert (owner->refcount > 0);
  owner->refcount += 1;

  return owner;
}

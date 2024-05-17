_dbus_atomic_get (DBusAtomic *atomic)
{
   
  MemoryBarrier ();
  return atomic->value;
}

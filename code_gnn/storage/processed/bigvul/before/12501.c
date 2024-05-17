bus_owner_set_flags (BusOwner *owner,
                     dbus_uint32_t flags)
{
   owner->allow_replacement = 
        (flags & DBUS_NAME_FLAG_ALLOW_REPLACEMENT) != FALSE;

   owner->do_not_queue =
        (flags & DBUS_NAME_FLAG_DO_NOT_QUEUE) != FALSE;
}

_dbus_header_set_serial (DBusHeader    *header,
                         dbus_uint32_t  serial)
{
   
  _dbus_assert (_dbus_header_get_serial (header) == 0 ||
                serial == 0);

  _dbus_marshal_set_uint32 (&header->data,
                            SERIAL_OFFSET,
			    serial,
                            header->byte_order);
}

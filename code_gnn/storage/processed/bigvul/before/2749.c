_dbus_header_get_message_type (DBusHeader *header)
{
  int type;

  type = _dbus_string_get_byte (&header->data, TYPE_OFFSET);
  _dbus_assert (type != DBUS_MESSAGE_TYPE_INVALID);

  return type;
}

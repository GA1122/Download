correct_header_padding (DBusHeader *header)
{
  int unpadded_len;

  _dbus_assert (header->padding == 7);

  _dbus_string_shorten (&header->data, header->padding);
  unpadded_len = _dbus_string_get_length (&header->data);

  if (!_dbus_string_align_length (&header->data, 8))
    _dbus_assert_not_reached ("couldn't pad header though enough padding was preallocated");

  header->padding = _dbus_string_get_length (&header->data) - unpadded_len;
}

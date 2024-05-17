_dbus_header_delete_field (DBusHeader *header,
                           int         field)
{
  DBusTypeReader reader;
  DBusTypeReader realign_root;

  if (_dbus_header_cache_known_nonexistent (header, field))
    return TRUE;  

   
  if (!find_field_for_modification (header, field,
                                    &reader, &realign_root))
    return TRUE;  

  if (!reserve_header_padding (header))
    return FALSE;

  if (!_dbus_type_reader_delete (&reader,
                                 &realign_root))
    return FALSE;

  correct_header_padding (header);

  _dbus_header_cache_invalidate_all (header);

  _dbus_assert (!_dbus_header_cache_check (header, field));  

  return TRUE;
}

iter_recurse (DBusMessageDataIter *iter)
{
  iter->depth += 1;
  _dbus_assert (iter->depth < _DBUS_MESSAGE_DATA_MAX_NESTING);
  _dbus_assert (iter->sequence_nos[iter->depth] >= 0);
}

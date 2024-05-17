_dbus_write_socket (int               fd,
                    const DBusString *buffer,
                    int               start,
                    int               len)
{
  const char *data;
  int bytes_written;

  data = _dbus_string_get_const_data_len (buffer, start, len);

 again:

  _dbus_verbose ("send: len=%d fd=%d\n", len, fd);
  bytes_written = send (fd, data, len, 0);

  if (bytes_written == SOCKET_ERROR)
    {
      DBUS_SOCKET_SET_ERRNO();
      _dbus_verbose ("send: failed: %s\n", _dbus_strerror_from_errno ());
      bytes_written = -1;
    }
    else
      _dbus_verbose ("send: = %d\n", bytes_written);

  if (bytes_written < 0 && errno == EINTR)
    goto again;
    
#if 0
  if (bytes_written > 0)
    _dbus_verbose_bytes_of_string (buffer, start, bytes_written);
#endif

  return bytes_written;
}

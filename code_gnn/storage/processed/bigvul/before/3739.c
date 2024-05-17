_dbus_generate_random_bytes (DBusString *str,
                             int         n_bytes)
{
  int old_len;
  int fd;

   

  old_len = _dbus_string_get_length (str);
  fd = -1;

   
  fd = open ("/dev/urandom", O_RDONLY);
  if (fd < 0)
    return _dbus_generate_pseudorandom_bytes (str, n_bytes);

  _dbus_verbose ("/dev/urandom fd %d opened\n", fd);

  if (_dbus_read (fd, str, n_bytes) != n_bytes)
    {
      _dbus_close (fd, NULL);
      _dbus_string_set_length (str, old_len);
      return _dbus_generate_pseudorandom_bytes (str, n_bytes);
    }

  _dbus_verbose ("Read %d bytes from /dev/urandom\n",
                 n_bytes);

  _dbus_close (fd, NULL);

  return TRUE;
}
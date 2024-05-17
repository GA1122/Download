_dbus_read_credentials_socket  (int              handle,
                                DBusCredentials *credentials,
                                DBusError       *error)
{
  int bytes_read = 0;
  DBusString buf;
  
  if (_dbus_string_init(&buf))
    {
      bytes_read = _dbus_read_socket(handle, &buf, 1 );

      if (bytes_read > 0) 
        _dbus_verbose("got one zero byte from server");

      _dbus_string_free(&buf);
    }

  _dbus_credentials_add_from_current_process (credentials);
  _dbus_verbose("FIXME: get faked credentials from current process");

  return TRUE;
}

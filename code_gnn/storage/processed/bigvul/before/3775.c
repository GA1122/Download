_dbus_credentials_add_from_user (DBusCredentials  *credentials,
                                     const DBusString *username)
{
  return _dbus_credentials_add_windows_sid (credentials,
                    _dbus_string_get_const_data(username));
}

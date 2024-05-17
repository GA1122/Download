_dbus_credentials_add_from_current_process (DBusCredentials *credentials)
{
   
  _dbus_assert (sizeof (pid_t) <= sizeof (dbus_pid_t));
  _dbus_assert (sizeof (uid_t) <= sizeof (dbus_uid_t));
  _dbus_assert (sizeof (gid_t) <= sizeof (dbus_gid_t));

  if (!_dbus_credentials_add_unix_pid(credentials, _dbus_getpid()))
    return FALSE;
  if (!_dbus_credentials_add_unix_uid(credentials, _dbus_geteuid()))
    return FALSE;

  return TRUE;
}

_dbus_credentials_add_from_current_process (DBusCredentials *credentials)
{
  dbus_bool_t retval = FALSE;
  char *sid = NULL;

  if (!_dbus_getsid(&sid))
    goto failed;

  if (!_dbus_credentials_add_unix_pid(credentials, _dbus_getpid()))
    goto failed;

  if (!_dbus_credentials_add_windows_sid (credentials,sid))
    goto failed;

  retval = TRUE;
  goto end;
failed:
  retval = FALSE;
end:
  if (sid)
    LocalFree(sid);

  return retval;
}

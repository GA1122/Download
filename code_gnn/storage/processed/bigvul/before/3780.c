_dbus_delete_directory (const DBusString *filename,
                        DBusError        *error)
{
  const char *filename_c;

  _DBUS_ASSERT_ERROR_IS_CLEAR (error);

  filename_c = _dbus_string_get_const_data (filename);

  if (RemoveDirectoryA (filename_c) == 0)
    {
      char *emsg = _dbus_win_error_string (GetLastError ());
      dbus_set_error (error, _dbus_win_error_from_last_error (),
                      "Failed to remove directory %s: %s",
                      filename_c, emsg);
      _dbus_win_free_error_string (emsg);
      return FALSE;
    }

  return TRUE;
}

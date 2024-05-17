_dbus_get_standard_system_servicedirs (DBusList **dirs)
{
   
  static const char standard_search_path[] =
    "/usr/local/share:"
    "/usr/share:"
    DBUS_DATADIR ":"
    "/lib";
  DBusString servicedir_path;

  _dbus_string_init_const (&servicedir_path, standard_search_path);

  return _dbus_split_paths_and_append (&servicedir_path,
                                       DBUS_UNIX_STANDARD_SYSTEM_SERVICEDIR,
                                       dirs);
}

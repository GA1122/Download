dbus_g_proxy_manager_lookup_name_owner (DBusGProxyManager        *manager,
					const char               *name,
					DBusGProxyNameOwnerInfo **info,
					const char              **owner)
{
  DBusGProxyNameOwnerForeachData foreach_data;

  foreach_data.name = name;
  foreach_data.owner = NULL;
  foreach_data.info = NULL;
  
  g_hash_table_foreach (manager->owner_names, name_owner_foreach, &foreach_data);

  *info = foreach_data.info;
  *owner = foreach_data.owner;
  return *info != NULL;
}

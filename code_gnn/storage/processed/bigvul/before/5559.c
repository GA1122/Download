name_owner_foreach (gpointer key, gpointer val, gpointer data)
{
  const char *owner;
  DBusGProxyNameOwnerForeachData *foreach_data;
  GSList *names;
  GSList *link;

  owner = key;
  names = val;
  foreach_data = data;

  if (foreach_data->owner != NULL)
    return;

  g_assert (foreach_data->info == NULL);

  link = g_slist_find_custom (names, foreach_data->name, find_name_in_info);
  if (link)
    {
      foreach_data->owner = owner;
      foreach_data->info = link->data;
    }
}

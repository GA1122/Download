dbus_g_proxy_manager_replace_name_owner (DBusGProxyManager  *manager,
					 const char         *name,
					 const char         *prev_owner,
					 const char         *new_owner)
{
  GSList *names;
	  
  if (prev_owner[0] == '\0')
    {
      GSList *tmp;
      GSList *removed;

       

      removed = NULL;

      for (tmp = manager->unassociated_proxies; tmp ; tmp = tmp->next)
	{
	  DBusGProxy *proxy = tmp->data;
	  DBusGProxyPrivate *priv = DBUS_G_PROXY_GET_PRIVATE(proxy);

	  if (!strcmp (priv->name, name))
	    {
	      removed = g_slist_prepend (removed, tmp);
	      
	      dbus_g_proxy_manager_monitor_name_owner (manager, new_owner, name);
	      priv->associated = TRUE;
	    }
	}

      for (tmp = removed; tmp; tmp = tmp->next)
	manager->unassociated_proxies = g_slist_delete_link (manager->unassociated_proxies, tmp->data);
      g_slist_free (removed);
    }
  else
    {
      DBusGProxyNameOwnerInfo *info;
      GSList *link;

        

      names = g_hash_table_lookup (manager->owner_names, prev_owner);

      info = NULL;
      if (names != NULL)
        {
	  link = g_slist_find_custom (names, name, find_name_in_info);

	  if (link != NULL)
	    {
	      info = link->data;
	  
	      names = g_slist_delete_link (names, link);

              if (names == NULL)
                {
                  g_hash_table_remove (manager->owner_names, prev_owner);
                }
              else
                {
                  g_hash_table_insert (manager->owner_names,
                                       g_strdup (prev_owner), names);
                }
            }
        }

      if (new_owner[0] == '\0')
	{
	  DBusGProxyUnassociateData data;
	  GSList *tmp;

	  data.name = name;
	  data.destroyed = NULL;

	   
	  g_hash_table_foreach (manager->proxy_lists,
				unassociate_proxies, &data);

	  UNLOCK_MANAGER (manager);

           
	  for (tmp = data.destroyed; tmp; tmp = tmp->next)
            if (tmp->data != NULL)
              {
                g_object_remove_weak_pointer (G_OBJECT (tmp->data),
                    &(tmp->data));
                dbus_g_proxy_destroy (tmp->data);
              }
	  g_slist_free (data.destroyed);

	  LOCK_MANAGER (manager);

	  if (info)
	    {
	      g_free (info->name);
	      g_free (info);
	    }
	}
      else if (info)
	{
	  insert_nameinfo (manager, new_owner, info);
	}
    }
}

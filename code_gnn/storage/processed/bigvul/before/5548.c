dbus_g_proxy_manager_unregister (DBusGProxyManager *manager,
                                DBusGProxy        *proxy)
{
  DBusGProxyList *list;
  DBusGProxyPrivate *priv = DBUS_G_PROXY_GET_PRIVATE(proxy);
  char *tri;
  
  LOCK_MANAGER (manager);

#ifndef G_DISABLE_CHECKS
  if (manager->proxy_lists == NULL)
    {
      g_warning ("Trying to unregister a proxy but there aren't any registered");
      return;
    }
#endif

  tri = tristring_from_proxy (proxy);
  
  list = g_hash_table_lookup (manager->proxy_lists, tri);

#ifndef G_DISABLE_CHECKS
  if (list == NULL)
    {
      g_warning ("Trying to unregister a proxy but it isn't registered");
      return;
    }
#endif

  g_assert (g_slist_find (list->proxies, proxy) != NULL);
  
  list->proxies = g_slist_remove (list->proxies, proxy);

  g_assert (g_slist_find (list->proxies, proxy) == NULL);

  if (!priv->for_owner)
    {
      if (!priv->associated)
	{
	  GSList *link;

	  if (priv->name_call != 0)
	    {
	      dbus_g_proxy_cancel_call (manager->bus_proxy, priv->name_call);
	      priv->name_call = 0;
	    }
	  else
	    {
              link = g_slist_find (manager->unassociated_proxies, proxy);
              if (link != NULL)
                {
                  manager->unassociated_proxies = g_slist_delete_link (
                      manager->unassociated_proxies, link);
                }
	    }
	}
      else
	{
	  g_assert (priv->name_call == 0);
	  
	  dbus_g_proxy_manager_unmonitor_name_owner (manager, priv->name);
	}
    }

  if (list->proxies == NULL)
    {
      char *rule;
      g_hash_table_remove (manager->proxy_lists,
                           tri);

      rule = g_proxy_get_signal_match_rule (proxy);
      dbus_bus_remove_match (manager->connection,
                             rule, NULL);
      g_free (rule);

      if (priv->name)
        {
          guint *refcount;
          refcount = g_hash_table_lookup (manager->owner_match_rules, priv->name);
          (*refcount)--;

          if (*refcount == 0)
            {
              rule = get_owner_match_rule (priv->name);
              dbus_bus_remove_match (manager->connection,
                                     rule, NULL);
              g_free (rule);
              g_hash_table_remove (manager->owner_match_rules, priv->name);
            }
        }
    }
  
  if (g_hash_table_size (manager->proxy_lists) == 0)
    {
      g_hash_table_destroy (manager->proxy_lists);
      manager->proxy_lists = NULL;
    }

  if (manager->owner_match_rules != NULL &&
      g_hash_table_size (manager->owner_match_rules) == 0)
    {
      g_hash_table_destroy (manager->owner_match_rules);
      manager->owner_match_rules = NULL;
    }

  g_free (tri);
      
  UNLOCK_MANAGER (manager);
}

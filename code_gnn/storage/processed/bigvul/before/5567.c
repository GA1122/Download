unassociate_proxies (gpointer key, gpointer val, gpointer user_data)
{
  DBusGProxyList *list;
  const char *name;
  GSList *tmp;
  DBusGProxyUnassociateData *data;

  list = val;
  data = user_data;
  name = data->name;
  
  for (tmp = list->proxies; tmp; tmp = tmp->next)
    {
      DBusGProxy *proxy = DBUS_G_PROXY (tmp->data);
      DBusGProxyPrivate *priv = DBUS_G_PROXY_GET_PRIVATE(proxy);
      DBusGProxyManager *manager;

      manager = priv->manager;

      if (priv->name != NULL && !strcmp (priv->name, name))
	{
	  if (!priv->for_owner)
	    {
	       
	      if (priv->name_call)
		dbus_g_proxy_cancel_call (manager->bus_proxy, priv->name_call);

	      priv->name_call = NULL;

	      priv->associated = FALSE;
	      manager->unassociated_proxies = g_slist_prepend (manager->unassociated_proxies, proxy);
	    }
	  else
	    {
	      data->destroyed = g_slist_prepend (data->destroyed, proxy);
               
              g_object_add_weak_pointer (G_OBJECT (proxy),
                  &(data->destroyed->data));
	    }
	}
    }
}

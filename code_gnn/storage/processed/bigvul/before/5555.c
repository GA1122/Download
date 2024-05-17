got_name_owner_cb (DBusGProxy       *bus_proxy,
		   DBusGProxyCall   *call,
		   void             *user_data)
{
  DBusGProxy *proxy = user_data;
  DBusGProxyPrivate *priv = DBUS_G_PROXY_GET_PRIVATE(proxy);
  GError *error;
  char *owner;

  error = NULL;
  owner = NULL;

  LOCK_MANAGER (priv->manager);

  if (!dbus_g_proxy_end_call (bus_proxy, call, &error,
			      G_TYPE_STRING, &owner,
			      G_TYPE_INVALID))
    {
      if (error->domain == DBUS_GERROR && error->code == DBUS_GERROR_NAME_HAS_NO_OWNER)
	{
	  priv->manager->unassociated_proxies = g_slist_prepend (priv->manager->unassociated_proxies, proxy);
	}
      else if (error->domain == DBUS_GERROR && error->code == DBUS_GERROR_REMOTE_EXCEPTION)
	g_warning ("Couldn't get name owner (%s): %s",
		   dbus_g_error_get_name (error),
		   error->message);
      else
	g_warning ("Couldn't get name owner (code %d): %s",
                   error->code, error->message);
      g_clear_error (&error);
      goto out;
    }
  else
    {
      dbus_g_proxy_manager_monitor_name_owner (priv->manager, owner, priv->name);
      priv->associated = TRUE;
    }

 out:
  priv->name_call = NULL;
  UNLOCK_MANAGER (priv->manager);
  g_free (owner);
}

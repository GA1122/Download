flatpak_proxy_start (FlatpakProxy *proxy, GError **error)
{
  GSocketAddress *address;
  gboolean res;

  unlink (proxy->socket_path);

  address = g_unix_socket_address_new (proxy->socket_path);

  error = NULL;
  res = g_socket_listener_add_address (G_SOCKET_LISTENER (proxy),
                                       address,
                                       G_SOCKET_TYPE_STREAM,
                                       G_SOCKET_PROTOCOL_DEFAULT,
                                       NULL,  
                                       NULL,  
                                       error);
  g_object_unref (address);

  if (!res)
    return FALSE;


  g_socket_service_start (G_SOCKET_SERVICE (proxy));
  return TRUE;
}
g_proxy_list_free (DBusGProxyList *list)
{
   
  g_slist_free (list->proxies);  

  g_free (list);
}

flatpak_proxy_client_get_policy (FlatpakProxyClient *client, const char *source)
{
  if (source == NULL)
    return FLATPAK_POLICY_TALK;  

  if (source[0] == ':')
    return GPOINTER_TO_UINT (g_hash_table_lookup (client->unique_id_policy, source));

  return flatpak_proxy_get_policy (client->proxy, source);
}
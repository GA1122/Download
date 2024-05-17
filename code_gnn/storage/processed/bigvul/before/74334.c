inf_gtk_certificate_manager_set_known_hosts(InfGtkCertificateManager* manager,
                                            const gchar* known_hosts_file)
{
  InfGtkCertificateManagerPrivate* priv;
  priv = INF_GTK_CERTIFICATE_MANAGER_PRIVATE(manager);

   
  g_assert(priv->queries == NULL);

  g_free(priv->known_hosts_file);
  priv->known_hosts_file = g_strdup(known_hosts_file);
}

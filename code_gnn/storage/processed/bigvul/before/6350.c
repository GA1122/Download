nm_ip4_config_export (NMIP4Config *config)
{
	NMIP4ConfigPrivate *priv = NM_IP4_CONFIG_GET_PRIVATE (config);
	static guint32 counter = 0;

	if (!priv->path) {
		priv->path = g_strdup_printf (NM_DBUS_PATH "/IP4Config/%d", counter++);
		nm_dbus_manager_register_object (nm_dbus_manager_get (), priv->path, config);
	}
}

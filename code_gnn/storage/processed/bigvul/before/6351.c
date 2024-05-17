nm_ip4_config_get_dbus_path (const NMIP4Config *config)
{
	NMIP4ConfigPrivate *priv = NM_IP4_CONFIG_GET_PRIVATE (config);

	return priv->path;
}

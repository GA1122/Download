nm_ip4_config_capture (int ifindex, gboolean capture_resolv_conf)
{
	NMIP4Config *config;
	NMIP4ConfigPrivate *priv;
	guint i;
	guint32 lowest_metric = G_MAXUINT32;
	guint32 old_gateway = 0;
	gboolean has_gateway = FALSE;

	 
	if (nm_platform_link_get_master (ifindex) > 0)
		return NULL;

	config = nm_ip4_config_new (ifindex);
	priv = NM_IP4_CONFIG_GET_PRIVATE (config);

	g_array_unref (priv->addresses);
	g_array_unref (priv->routes);

	priv->addresses = nm_platform_ip4_address_get_all (ifindex);
	priv->routes = nm_platform_ip4_route_get_all (ifindex, NM_PLATFORM_GET_ROUTE_MODE_ALL);

	 
	old_gateway = priv->gateway;
	for (i = 0; i < priv->routes->len; i++) {
		const NMPlatformIP4Route *route = &g_array_index (priv->routes, NMPlatformIP4Route, i);

		if (NM_PLATFORM_IP_ROUTE_IS_DEFAULT (route)) {
			if (route->metric < lowest_metric) {
				priv->gateway = route->gateway;
				lowest_metric = route->metric;
			}
			has_gateway = TRUE;
			 
			g_array_remove_index (priv->routes, i);
			i--;
		}
	}

	 
	if (has_gateway) {
		for (i = 0; i < priv->routes->len; i++) {
			const NMPlatformIP4Route *route = &g_array_index (priv->routes, NMPlatformIP4Route, i);

			if (   (route->plen == 32)
			    && (route->network == priv->gateway)
			    && (route->gateway == 0)) {
				g_array_remove_index (priv->routes, i);
				i--;
			}
		}
	}

	 
	if (priv->addresses->len && has_gateway && capture_resolv_conf) {
		if (nm_ip4_config_capture_resolv_conf (priv->nameservers, NULL))
			_NOTIFY (config, PROP_NAMESERVERS);
	}

	 
	_NOTIFY (config, PROP_ADDRESS_DATA);
	_NOTIFY (config, PROP_ROUTE_DATA);
	_NOTIFY (config, PROP_ADDRESSES);
	_NOTIFY (config, PROP_ROUTES);
	if (priv->gateway != old_gateway)
		_NOTIFY (config, PROP_GATEWAY);

	return config;
}

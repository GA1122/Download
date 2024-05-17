  device_has_capability (NMDevice *self, NMDeviceCapabilities caps)
  {
 {
 	static guint32 devcount = 0;
 	NMDevicePrivate *priv;
 
 	g_return_if_fail (NM_IS_DEVICE (self));
 
 	priv = NM_DEVICE_GET_PRIVATE (self);
 	g_return_if_fail (priv->path == NULL);
 
 	priv->path = g_strdup_printf ("/org/freedesktop/NetworkManager/Devices/%d", devcount++);
 	_LOGI (LOGD_DEVICE, "exported as %s", priv->path);
 	nm_dbus_manager_register_object (nm_dbus_manager_get (), priv->path, self);
 }
 
 const char *
 nm_device_get_path (NMDevice *self)
 {
 	g_return_val_if_fail (self != NULL, NULL);
 
 	return NM_DEVICE_GET_PRIVATE (self)->path;
 }
 
 const char *
 nm_device_get_udi (NMDevice *self)
 {
 	g_return_val_if_fail (self != NULL, NULL);
 
 	return NM_DEVICE_GET_PRIVATE (self)->udi;
 }
 
 const char *
 nm_device_get_iface (NMDevice *self)
 {
 	g_return_val_if_fail (NM_IS_DEVICE (self), 0);
 
 	return NM_DEVICE_GET_PRIVATE (self)->iface;
 }
 
 int
 nm_device_get_ifindex (NMDevice *self)
 {
 	g_return_val_if_fail (self != NULL, 0);
 
 	return NM_DEVICE_GET_PRIVATE (self)->ifindex;
 }
 
 gboolean
 nm_device_is_software (NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	return priv->is_software;
 }
 
 const char *
 nm_device_get_ip_iface (NMDevice *self)
 {
 	NMDevicePrivate *priv;
 
 	g_return_val_if_fail (self != NULL, NULL);
 
 	priv = NM_DEVICE_GET_PRIVATE (self);
 	 
 	return priv->ip_iface ? priv->ip_iface : priv->iface;
 }
 
 int
 nm_device_get_ip_ifindex (NMDevice *self)
 {
 	NMDevicePrivate *priv;
 
 	g_return_val_if_fail (self != NULL, 0);
 
 	priv = NM_DEVICE_GET_PRIVATE (self);
 	 
 	return priv->ip_iface ? priv->ip_ifindex : priv->ifindex;
 }
 
 void
 nm_device_set_ip_iface (NMDevice *self, const char *iface)
 {
 	NMDevicePrivate *priv;
 	char *old_ip_iface;
 
 	g_return_if_fail (NM_IS_DEVICE (self));
 
 	priv = NM_DEVICE_GET_PRIVATE (self);
 	if (!g_strcmp0 (iface, priv->ip_iface))
 		return;
 
 	old_ip_iface = priv->ip_iface;
 	priv->ip_ifindex = 0;
 
 	priv->ip_iface = g_strdup (iface);
 	if (priv->ip_iface) {
 		priv->ip_ifindex = nm_platform_link_get_ifindex (priv->ip_iface);
 		if (priv->ip_ifindex > 0) {
 			if (nm_platform_check_support_user_ipv6ll ())
 				nm_platform_link_set_user_ipv6ll_enabled (priv->ip_ifindex, TRUE);
 
 			if (!nm_platform_link_is_up (priv->ip_ifindex))
 				nm_platform_link_set_up (priv->ip_ifindex);
 		} else {
 			 
 			_LOGW (LOGD_HW, "failed to look up interface index");
 		}
 	}
 
 	 
 	g_hash_table_remove_all (priv->ip6_saved_properties);
 
 	 
 	if (g_strcmp0 (old_ip_iface, priv->ip_iface))
 		g_object_notify (G_OBJECT (self), NM_DEVICE_IP_IFACE);
 	g_free (old_ip_iface);
 }
 
 static gboolean
 get_ip_iface_identifier (NMDevice *self, NMUtilsIPv6IfaceId *out_iid)
 {
 	NMLinkType link_type;
 	const guint8 *hwaddr = NULL;
 	size_t hwaddr_len = 0;
 	int ifindex;
 	gboolean success;
 
 	 
 	ifindex = nm_device_get_ip_ifindex (self);
 	g_assert (ifindex);
 
 	link_type = nm_platform_link_get_type (ifindex);
 	g_return_val_if_fail (link_type > NM_LINK_TYPE_UNKNOWN, 0);
 
 	hwaddr = nm_platform_link_get_address (ifindex, &hwaddr_len);
 	if (!hwaddr_len)
 		return FALSE;
 
 	success = nm_utils_get_ipv6_interface_identifier (link_type,
 	                                                  hwaddr,
 	                                                  hwaddr_len,
 	                                                  out_iid);
 	if (!success) {
 		_LOGW (LOGD_HW, "failed to generate interface identifier "
 		       "for link type %u hwaddr_len %zu", link_type, hwaddr_len);
 	}
 	return success;
 }
 
 static gboolean
 nm_device_get_ip_iface_identifier (NMDevice *self, NMUtilsIPv6IfaceId *iid)
 {
 	return NM_DEVICE_GET_CLASS (self)->get_ip_iface_identifier (self, iid);
 }
 
 const char *
 nm_device_get_driver (NMDevice *self)
 {
 	g_return_val_if_fail (self != NULL, NULL);
 
 	return NM_DEVICE_GET_PRIVATE (self)->driver;
 }
 
 const char *
 nm_device_get_driver_version (NMDevice *self)
 {
 	g_return_val_if_fail (self != NULL, NULL);
 
 	return NM_DEVICE_GET_PRIVATE (self)->driver_version;
 }
 
 NMDeviceType
 nm_device_get_device_type (NMDevice *self)
 {
 	g_return_val_if_fail (NM_IS_DEVICE (self), NM_DEVICE_TYPE_UNKNOWN);
 
 	return NM_DEVICE_GET_PRIVATE (self)->type;
 }
 
 
  
 int
 nm_device_get_priority (NMDevice *self)
 {
 	g_return_val_if_fail (NM_IS_DEVICE (self), 1000);
 
 	 
 
 	switch (nm_device_get_device_type (self)) {
 	 
 	case NM_DEVICE_TYPE_ETHERNET:
 		return 100;
 	case NM_DEVICE_TYPE_INFINIBAND:
 		return 150;
 	case NM_DEVICE_TYPE_ADSL:
 		return 200;
 	case NM_DEVICE_TYPE_WIMAX:
 		return 250;
 	case NM_DEVICE_TYPE_BOND:
 		return 300;
 	case NM_DEVICE_TYPE_TEAM:
 		return 350;
 	case NM_DEVICE_TYPE_VLAN:
 		return 400;
 	case NM_DEVICE_TYPE_BRIDGE:
 		return 425;
 	case NM_DEVICE_TYPE_MODEM:
 		return 450;
 	case NM_DEVICE_TYPE_BT:
 		return 550;
 	case NM_DEVICE_TYPE_WIFI:
 		return 600;
 	case NM_DEVICE_TYPE_OLPC_MESH:
 		return 650;
 	case NM_DEVICE_TYPE_GENERIC:
 		return 950;
 	case NM_DEVICE_TYPE_UNKNOWN:
 		return 10000;
 	case NM_DEVICE_TYPE_UNUSED1:
 	case NM_DEVICE_TYPE_UNUSED2:
 		 
 		break;
 	}
 	return 11000;
 }
 
 guint32
 nm_device_get_ip4_route_metric (NMDevice *self)
 {
 	NMConnection *connection;
 	gint64 route_metric = -1;
 
 	g_return_val_if_fail (NM_IS_DEVICE (self), G_MAXUINT32);
 
 	connection = nm_device_get_connection (self);
 
 	if (connection)
 		route_metric = nm_setting_ip_config_get_route_metric (nm_connection_get_setting_ip4_config (connection));
 
 	return route_metric >= 0 ? route_metric : nm_device_get_priority (self);
 }
 
 guint32
 nm_device_get_ip6_route_metric (NMDevice *self)
 {
 	NMConnection *connection;
 	gint64 route_metric = -1;
 
 	g_return_val_if_fail (NM_IS_DEVICE (self), G_MAXUINT32);
 
 	connection = nm_device_get_connection (self);
 
 	if (connection)
 		route_metric = nm_setting_ip_config_get_route_metric (nm_connection_get_setting_ip6_config (connection));
 
 	return route_metric >= 0 ? route_metric : nm_device_get_priority (self);
 }
 
 const NMPlatformIP4Route *
 nm_device_get_ip4_default_route (NMDevice *self, gboolean *out_is_assumed)
 {
 	NMDevicePrivate *priv;
 
 	g_return_val_if_fail (NM_IS_DEVICE (self), NULL);
 
 	priv = NM_DEVICE_GET_PRIVATE (self);
 
 	if (out_is_assumed)
 		*out_is_assumed = priv->default_route.v4_is_assumed;
 
 	return priv->default_route.v4_has ? &priv->default_route.v4 : NULL;
 }
 
 const NMPlatformIP6Route *
 nm_device_get_ip6_default_route (NMDevice *self, gboolean *out_is_assumed)
 {
 	NMDevicePrivate *priv;
 
 	g_return_val_if_fail (NM_IS_DEVICE (self), NULL);
 
 	priv = NM_DEVICE_GET_PRIVATE (self);
 
 	if (out_is_assumed)
 		*out_is_assumed = priv->default_route.v6_is_assumed;
 
 	return priv->default_route.v6_has ? &priv->default_route.v6 : NULL;
 }
 
 const char *
 nm_device_get_type_desc (NMDevice *self)
 {
 	g_return_val_if_fail (self != NULL, NULL);
 
 	return NM_DEVICE_GET_PRIVATE (self)->type_desc;
 }
 
 gboolean
 nm_device_has_carrier (NMDevice *self)
 {
 	return NM_DEVICE_GET_PRIVATE (self)->carrier;
 }
 
 NMActRequest *
 nm_device_get_act_request (NMDevice *self)
 {
 	g_return_val_if_fail (self != NULL, NULL);
 
 	return NM_DEVICE_GET_PRIVATE (self)->act_request;
 }
 
 NMConnection *
 nm_device_get_connection (NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	return priv->act_request ? nm_act_request_get_connection (priv->act_request) : NULL;
 }
 
 RfKillType
 nm_device_get_rfkill_type (NMDevice *self)
 {
 	g_return_val_if_fail (NM_IS_DEVICE (self), FALSE);
 
 	return NM_DEVICE_GET_PRIVATE (self)->rfkill_type;
 }
 
 static const char *
 nm_device_get_physical_port_id (NMDevice *self)
 {
 	return NM_DEVICE_GET_PRIVATE (self)->physical_port_id;
 }
 
  
 
 static gboolean
 nm_device_uses_generated_assumed_connection (NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	NMConnection *connection;
 
 	if (   priv->act_request
 	    && nm_active_connection_get_assumed (NM_ACTIVE_CONNECTION (priv->act_request))) {
 		connection = nm_act_request_get_connection (priv->act_request);
 		if (   connection
 		    && nm_settings_connection_get_nm_generated_assumed (NM_SETTINGS_CONNECTION (connection)))
 			return TRUE;
 	}
 	return FALSE;
 }
 
 gboolean
 nm_device_uses_assumed_connection (NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	if (   priv->act_request
 	    && nm_active_connection_get_assumed (NM_ACTIVE_CONNECTION (priv->act_request)))
 		return TRUE;
 	return FALSE;
 }
 
 static SlaveInfo *
 find_slave_info (NMDevice *self, NMDevice *slave)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	SlaveInfo *info;
 	GSList *iter;
 
 	for (iter = priv->slaves; iter; iter = g_slist_next (iter)) {
 		info = iter->data;
 		if (info->slave == slave)
 			return info;
 	}
 	return NULL;
 }
 
 static void
 free_slave_info (SlaveInfo *info)
 {
 	g_signal_handler_disconnect (info->slave, info->watch_id);
 	g_clear_object (&info->slave);
 	memset (info, 0, sizeof (*info));
 	g_free (info);
 }
 
  
 static gboolean
 nm_device_enslave_slave (NMDevice *self, NMDevice *slave, NMConnection *connection)
 {
 	SlaveInfo *info;
 	gboolean success = FALSE;
 	gboolean configure;
 
 	g_return_val_if_fail (self != NULL, FALSE);
 	g_return_val_if_fail (slave != NULL, FALSE);
 	g_return_val_if_fail (NM_DEVICE_GET_CLASS (self)->enslave_slave != NULL, FALSE);
 
 	info = find_slave_info (self, slave);
 	if (!info)
 		return FALSE;
 
 	if (info->enslaved)
 		success = TRUE;
 	else {
 		configure = (info->configure && connection != NULL);
 		if (configure)
 			g_return_val_if_fail (nm_device_get_state (slave) >= NM_DEVICE_STATE_DISCONNECTED, FALSE);
 
 		success = NM_DEVICE_GET_CLASS (self)->enslave_slave (self, slave, connection, configure);
 		info->enslaved = success;
 	}
 
 	nm_device_slave_notify_enslave (info->slave, success);
 
 	 
 	nm_device_update_hw_address (self);
 
 	 
 	if (success) {
 		if (NM_DEVICE_GET_PRIVATE (self)->ip4_state == IP_WAIT)
 			nm_device_activate_stage3_ip4_start (self);
 
 		if (NM_DEVICE_GET_PRIVATE (self)->ip6_state == IP_WAIT)
 			nm_device_activate_stage3_ip6_start (self);
 	}
 
 	return success;
 }
 
  
 static gboolean
 nm_device_release_one_slave (NMDevice *self, NMDevice *slave, gboolean configure, NMDeviceStateReason reason)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	SlaveInfo *info;
 	gboolean success = FALSE;
 
 	g_return_val_if_fail (slave != NULL, FALSE);
 	g_return_val_if_fail (NM_DEVICE_GET_CLASS (self)->release_slave != NULL, FALSE);
 
 	info = find_slave_info (self, slave);
 	if (!info)
 		return FALSE;
 	priv->slaves = g_slist_remove (priv->slaves, info);
 
 	if (info->enslaved) {
 		success = NM_DEVICE_GET_CLASS (self)->release_slave (self, slave, configure);
 		 
 	}
 
 	if (!configure) {
 		g_warn_if_fail (reason == NM_DEVICE_STATE_REASON_NONE || reason == NM_DEVICE_STATE_REASON_REMOVED);
 		reason = NM_DEVICE_STATE_REASON_NONE;
 	} else if (reason == NM_DEVICE_STATE_REASON_NONE) {
 		g_warn_if_reached ();
 		reason = NM_DEVICE_STATE_REASON_UNKNOWN;
 	}
 	nm_device_slave_notify_release (info->slave, reason);
 
 	free_slave_info (info);
 
 	 
 	nm_device_update_hw_address (self);
 
 	return success;
 }
 
 static gboolean
 is_software_external (NMDevice *self)
 {
 	return   nm_device_is_software (self)
 	      && !nm_device_get_is_nm_owned (self);
 }
 
  
 void
 nm_device_finish_init (NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	g_assert (priv->initialized == FALSE);
 
 	 
 	if (   is_software_external (self)
 	    && !nm_platform_link_is_up (priv->ifindex)
 	    && priv->ifindex > 0)
 		nm_device_set_initial_unmanaged_flag (self, NM_UNMANAGED_EXTERNAL_DOWN, TRUE);
 
 	if (priv->master)
 		nm_device_enslave_slave (priv->master, self, NULL);
 
 	priv->initialized = TRUE;
 }
 
 static void
 carrier_changed (NMDevice *self, gboolean carrier)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	if (!nm_device_get_managed (self))
 		return;
 
 	nm_device_recheck_available_connections (self);
 
 	 
 	if (priv->ignore_carrier && !carrier)
 		return;
 
 	if (priv->is_master) {
 		 
 		if (!carrier)
 			return;
 
 		if (nm_device_activate_ip4_state_in_wait (self))
 			nm_device_activate_stage3_ip4_start (self);
 		if (nm_device_activate_ip6_state_in_wait (self))
 			nm_device_activate_stage3_ip6_start (self);
 
 		return;
 	} else if (nm_device_get_enslaved (self) && !carrier) {
 		 
 		return;
 	}
 
 	if (carrier) {
 		g_warn_if_fail (priv->state >= NM_DEVICE_STATE_UNAVAILABLE);
 
 		if (priv->state == NM_DEVICE_STATE_UNAVAILABLE) {
 			nm_device_queue_state (self, NM_DEVICE_STATE_DISCONNECTED,
 			                       NM_DEVICE_STATE_REASON_CARRIER);
 		} else if (priv->state == NM_DEVICE_STATE_DISCONNECTED) {
 			 
 			nm_device_emit_recheck_auto_activate (self);
 		}
 	} else {
 		g_return_if_fail (priv->state >= NM_DEVICE_STATE_UNAVAILABLE);
 
 		if (priv->state == NM_DEVICE_STATE_UNAVAILABLE) {
 			if (nm_device_queued_state_peek (self) >= NM_DEVICE_STATE_DISCONNECTED)
 				nm_device_queued_state_clear (self);
 		} else {
 			nm_device_queue_state (self, NM_DEVICE_STATE_UNAVAILABLE,
 			                       NM_DEVICE_STATE_REASON_CARRIER);
 		}
 	}
 }
 
 #define LINK_DISCONNECT_DELAY 4
 
 static gboolean
 link_disconnect_action_cb (gpointer user_data)
 {
 	NMDevice *self = NM_DEVICE (user_data);
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	_LOGD (LOGD_DEVICE, "link disconnected (calling deferred action) (id=%u)", priv->carrier_defer_id);
 
 	priv->carrier_defer_id = 0;
 
 	_LOGI (LOGD_DEVICE, "link disconnected (calling deferred action)");
 
 	NM_DEVICE_GET_CLASS (self)->carrier_changed (self, FALSE);
 
 	return FALSE;
 }
 
 static void
 link_disconnect_action_cancel (NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	if (priv->carrier_defer_id) {
 		g_source_remove (priv->carrier_defer_id);
 		_LOGD (LOGD_DEVICE, "link disconnected (canceling deferred action) (id=%u)", priv->carrier_defer_id);
 		priv->carrier_defer_id = 0;
 	}
 }
 
 void
 nm_device_set_carrier (NMDevice *self, gboolean carrier)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	NMDeviceClass *klass = NM_DEVICE_GET_CLASS (self);
 	NMDeviceState state = nm_device_get_state (self);
 
 	if (priv->carrier == carrier)
 		return;
 
 	priv->carrier = carrier;
 	g_object_notify (G_OBJECT (self), NM_DEVICE_CARRIER);
 
 	if (priv->carrier) {
 		_LOGI (LOGD_DEVICE, "link connected");
 		link_disconnect_action_cancel (self);
 		klass->carrier_changed (self, TRUE);
 
 		if (priv->carrier_wait_id) {
 			g_source_remove (priv->carrier_wait_id);
 			priv->carrier_wait_id = 0;
 			nm_device_remove_pending_action (self, "carrier wait", TRUE);
 			_carrier_wait_check_queued_act_request (self);
 		}
 	} else if (state <= NM_DEVICE_STATE_DISCONNECTED) {
 		_LOGI (LOGD_DEVICE, "link disconnected");
 		klass->carrier_changed (self, FALSE);
 	} else {
 		_LOGI (LOGD_DEVICE, "link disconnected (deferring action for %d seconds)", LINK_DISCONNECT_DELAY);
 		priv->carrier_defer_id = g_timeout_add_seconds (LINK_DISCONNECT_DELAY,
 		                                                link_disconnect_action_cb, self);
 		_LOGD (LOGD_DEVICE, "link disconnected (deferring action for %d seconds) (id=%u)",
 		       LINK_DISCONNECT_DELAY, priv->carrier_defer_id);
 	}
 }
 
 static void
 update_for_ip_ifname_change (NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	g_hash_table_remove_all (priv->ip6_saved_properties);
 
 	if (priv->dhcp4_client) {
 		if (!nm_device_dhcp4_renew (self, FALSE)) {
 			nm_device_state_changed (self,
 			                         NM_DEVICE_STATE_FAILED,
 			                         NM_DEVICE_STATE_REASON_DHCP_FAILED);
 			return;
 		}
 	}
 	if (priv->dhcp6_client) {
 		if (!nm_device_dhcp6_renew (self, FALSE)) {
 			nm_device_state_changed (self,
 			                         NM_DEVICE_STATE_FAILED,
 			                         NM_DEVICE_STATE_REASON_DHCP_FAILED);
 			return;
 		}
 	}
 	if (priv->rdisc) {
 		 
 	}
 	if (priv->dnsmasq_manager) {
 		 
 	}
 }
 
 static void
 device_set_master (NMDevice *self, int ifindex)
 {
 	NMDevice *master;
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	master = nm_manager_get_device_by_ifindex (nm_manager_get (), ifindex);
 	if (master && NM_DEVICE_GET_CLASS (master)->enslave_slave) {
 		g_clear_object (&priv->master);
 		priv->master = g_object_ref (master);
 		nm_device_master_add_slave (master, self, FALSE);
 	} else if (master) {
 		_LOGI (LOGD_DEVICE, "enslaved to non-master-type device %s; ignoring",
 		       nm_device_get_iface (master));
 	} else {
 		_LOGW (LOGD_DEVICE, "enslaved to unknown device %d %s",
 		       ifindex,
 		       nm_platform_link_get_name (ifindex));
 	}
 }
 
 static void
 device_link_changed (NMDevice *self, NMPlatformLink *info)
 {
 	NMDeviceClass *klass = NM_DEVICE_GET_CLASS (self);
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	NMUtilsIPv6IfaceId token_iid;
 	gboolean ip_ifname_changed = FALSE;
 
 	if (info->udi && g_strcmp0 (info->udi, priv->udi)) {
 		 
 		g_free (priv->udi);
 		priv->udi = g_strdup (info->udi);
 		g_object_notify (G_OBJECT (self), NM_DEVICE_UDI);
 	}
 
 	 
 	if (priv->mtu != info->mtu) {
 		priv->mtu = info->mtu;
 		g_object_notify (G_OBJECT (self), NM_DEVICE_MTU);
 	}
 
 	if (info->name[0] && strcmp (priv->iface, info->name) != 0) {
 		_LOGI (LOGD_DEVICE, "interface index %d renamed iface from '%s' to '%s'",
 		       priv->ifindex, priv->iface, info->name);
 		g_free (priv->iface);
 		priv->iface = g_strdup (info->name);
 
 		 
 		ip_ifname_changed = !priv->ip_iface;
 
 		g_object_notify (G_OBJECT (self), NM_DEVICE_IFACE);
 		if (ip_ifname_changed)
 			g_object_notify (G_OBJECT (self), NM_DEVICE_IP_IFACE);
 
 		 
 		nm_device_recheck_available_connections (self);
 
 		 
 		nm_device_emit_recheck_auto_activate (self);
 	}
 
 	 
 	if (priv->enslaved && info->master != nm_device_get_ifindex (priv->master))
 		nm_device_release_one_slave (priv->master, self, FALSE, NM_DEVICE_STATE_REASON_NONE);
 	if (info->master && !priv->enslaved) {
 		device_set_master (self, info->master);
 		if (priv->master)
 			nm_device_enslave_slave (priv->master, self, NULL);
 	}
 
 	if (priv->rdisc && nm_platform_link_get_ipv6_token (priv->ifindex, &token_iid)) {
 		_LOGD (LOGD_DEVICE, "IPv6 tokenized identifier present on device %s", priv->iface);
 		if (nm_rdisc_set_iid (priv->rdisc, token_iid))
 			nm_rdisc_start (priv->rdisc);
 	}
 
 	if (klass->link_changed)
 		klass->link_changed (self, info);
 
 	 
 	if (ip_ifname_changed)
 		update_for_ip_ifname_change (self);
 
 	if (priv->up != info->up) {
 		priv->up = info->up;
 
 		 
 		g_assert (priv->ifindex > 0);
 		if (is_software_external (self)) {
 			gboolean external_down = nm_device_get_unmanaged_flag (self, NM_UNMANAGED_EXTERNAL_DOWN);
 
 			if (external_down && info->up) {
 				if (nm_device_get_state (self) < NM_DEVICE_STATE_DISCONNECTED) {
 					 
 					nm_device_queue_recheck_assume (self);
 
 					 
 					nm_device_set_unmanaged (self,
 					                         NM_UNMANAGED_EXTERNAL_DOWN,
 					                         FALSE,
 					                         NM_DEVICE_STATE_REASON_CONNECTION_ASSUMED);
 				} else {
 					 
 					priv->unmanaged_flags &= ~NM_UNMANAGED_EXTERNAL_DOWN;
 				}
 			} else if (!external_down && !info->up && nm_device_get_state (self) <= NM_DEVICE_STATE_DISCONNECTED) {
 				 
 				nm_device_set_unmanaged (self,
 				                         NM_UNMANAGED_EXTERNAL_DOWN,
 				                         TRUE,
 				                         NM_DEVICE_STATE_REASON_USER_REQUESTED);
 			}
 		}
 	}
 }
 
 static void
 device_ip_link_changed (NMDevice *self, NMPlatformLink *info)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	if (info->name[0] && g_strcmp0 (priv->ip_iface, info->name)) {
 		_LOGI (LOGD_DEVICE, "interface index %d renamed ip_iface (%d) from '%s' to '%s'",
 		       priv->ifindex, nm_device_get_ip_ifindex (self),
 		       priv->ip_iface, info->name);
 		g_free (priv->ip_iface);
 		priv->ip_iface = g_strdup (info->name);
 
 		g_object_notify (G_OBJECT (self), NM_DEVICE_IP_IFACE);
 		update_for_ip_ifname_change (self);
 	}
 }
 
 static void
 link_changed_cb (NMPlatform *platform,
                  int ifindex,
                  NMPlatformLink *info,
                  NMPlatformSignalChangeType change_type,
                  NMPlatformReason reason,
                  NMDevice *self)
 {
 	if (change_type != NM_PLATFORM_SIGNAL_CHANGED)
 		return;
 
 	 
 
 	if (ifindex == nm_device_get_ifindex (self))
 		device_link_changed (self, info);
 	else if (ifindex == nm_device_get_ip_ifindex (self))
 		device_ip_link_changed (self, info);
 }
 
 static void
 link_changed (NMDevice *self, NMPlatformLink *info)
 {
 	 
 	if (   device_has_capability (self, NM_DEVICE_CAP_CARRIER_DETECT)
 	    && !device_has_capability (self, NM_DEVICE_CAP_NONSTANDARD_CARRIER))
 		nm_device_set_carrier (self, info->connected);
 }
 
  
 gboolean
 nm_device_notify_component_added (NMDevice *self, GObject *component)
 {
 	if (NM_DEVICE_GET_CLASS (self)->component_added)
 		return NM_DEVICE_GET_CLASS (self)->component_added (self, component);
 	return FALSE;
 }
 
  
 gboolean
 nm_device_owns_iface (NMDevice *self, const char *iface)
 {
 	if (NM_DEVICE_GET_CLASS (self)->owns_iface)
 		return NM_DEVICE_GET_CLASS (self)->owns_iface (self, iface);
 	return FALSE;
 }
 
 NMConnection *
 nm_device_new_default_connection (NMDevice *self)
 {
 	if (NM_DEVICE_GET_CLASS (self)->new_default_connection)
 		return NM_DEVICE_GET_CLASS (self)->new_default_connection (self);
 	return NULL;
 }
 
 static void
 slave_state_changed (NMDevice *slave,
                      NMDeviceState slave_new_state,
                      NMDeviceState slave_old_state,
                      NMDeviceStateReason reason,
                      NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	gboolean release = FALSE;
 
 	_LOGD (LOGD_DEVICE, "slave %s state change %d (%s) -> %d (%s)",
 	       nm_device_get_iface (slave),
 	       slave_old_state,
 	       state_to_string (slave_old_state),
 	       slave_new_state,
 	       state_to_string (slave_new_state));
 
 	 
 	if (priv->state < NM_DEVICE_STATE_CONFIG)
 		return;
 
 	if (slave_new_state == NM_DEVICE_STATE_IP_CONFIG)
 		nm_device_enslave_slave (self, slave, nm_device_get_connection (slave));
 	else if (slave_new_state > NM_DEVICE_STATE_ACTIVATED)
 		release = TRUE;
 	else if (   slave_new_state <= NM_DEVICE_STATE_DISCONNECTED
 	         && slave_old_state > NM_DEVICE_STATE_DISCONNECTED) {
 		 
 		release = TRUE;
 	}
 
 	if (release) {
 		nm_device_release_one_slave (self, slave, TRUE, reason);
 		 
 		if (priv->slaves == NULL && priv->state == NM_DEVICE_STATE_ACTIVATED)
 			_LOGD (LOGD_DEVICE, "last slave removed; remaining activated");
 	}
 }
 
  
 static gboolean
 nm_device_master_add_slave (NMDevice *self, NMDevice *slave, gboolean configure)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	SlaveInfo *info;
 
 	g_return_val_if_fail (self != NULL, FALSE);
 	g_return_val_if_fail (slave != NULL, FALSE);
 	g_return_val_if_fail (NM_DEVICE_GET_CLASS (self)->enslave_slave != NULL, FALSE);
 
 	if (configure)
 		g_return_val_if_fail (nm_device_get_state (slave) >= NM_DEVICE_STATE_DISCONNECTED, FALSE);
 
 	if (!find_slave_info (self, slave)) {
 		info = g_malloc0 (sizeof (SlaveInfo));
 		info->slave = g_object_ref (slave);
 		info->configure = configure;
 		info->watch_id = g_signal_connect (slave, "state-changed",
 		                                   G_CALLBACK (slave_state_changed), self);
 		priv->slaves = g_slist_append (priv->slaves, info);
 	}
 	nm_device_queue_recheck_assume (self);
 
 	return TRUE;
 }
 
 
  
 GSList *
 nm_device_master_get_slaves (NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	GSList *slaves = NULL, *iter;
 
 	for (iter = priv->slaves; iter; iter = g_slist_next (iter))
 		slaves = g_slist_prepend (slaves, ((SlaveInfo *) iter->data)->slave);
 
 	return slaves;
 }
 
  
 NMDevice *
 nm_device_master_get_slave_by_ifindex (NMDevice *self, int ifindex)
 {
 	GSList *iter;
 
 	for (iter = NM_DEVICE_GET_PRIVATE (self)->slaves; iter; iter = g_slist_next (iter)) {
 		SlaveInfo *info = iter->data;
 
 		if (nm_device_get_ip_ifindex (info->slave) == ifindex)
 			return info->slave;
 	}
 	return NULL;
 }
 
  
 void
 nm_device_master_check_slave_physical_port (NMDevice *self, NMDevice *slave,
                                             guint64 log_domain)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	const char *slave_physical_port_id, *existing_physical_port_id;
 	SlaveInfo *info;
 	GSList *iter;
 
 	slave_physical_port_id = nm_device_get_physical_port_id (slave);
 	if (!slave_physical_port_id)
 		return;
 
 	for (iter = priv->slaves; iter; iter = iter->next) {
 		info = iter->data;
 		if (info->slave == slave)
 			continue;
 
 		existing_physical_port_id = nm_device_get_physical_port_id (info->slave);
 		if (!g_strcmp0 (slave_physical_port_id, existing_physical_port_id)) {
 			_LOGW (log_domain, "slave %s shares a physical port with existing slave %s",
 			       nm_device_get_ip_iface (slave),
 			       nm_device_get_ip_iface (info->slave));
 			 
 			return;
 		}
 	}
 }
 
  
 static void
 nm_device_master_release_slaves (NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	NMDeviceStateReason reason;
 
 	 
 	if (nm_device_uses_generated_assumed_connection (self))
 		return;
 
 	reason = priv->state_reason;
 	if (priv->state == NM_DEVICE_STATE_FAILED)
 		reason = NM_DEVICE_STATE_REASON_DEPENDENCY_FAILED;
 
 	while (priv->slaves) {
 		SlaveInfo *info = priv->slaves->data;
 
 		nm_device_release_one_slave (self, info->slave, TRUE, reason);
 	}
 }
 
  
 NMDevice *
 nm_device_get_master (NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	if (priv->enslaved)
 		return priv->master;
 	else
 		return NULL;
 }
 
  
 static void
 nm_device_slave_notify_enslave (NMDevice *self, gboolean success)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	NMConnection *connection = nm_device_get_connection (self);
 	gboolean activating = (priv->state == NM_DEVICE_STATE_IP_CONFIG);
 
 	g_assert (priv->master);
 
 	if (!priv->enslaved) {
 		if (success) {
 			if (activating) {
 				_LOGI (LOGD_DEVICE, "Activation: connection '%s' enslaved, continuing activation",
 				       nm_connection_get_id (connection));
 			} else
 				_LOGI (LOGD_DEVICE, "enslaved to %s", nm_device_get_iface (priv->master));
 
 			priv->enslaved = TRUE;
 			g_object_notify (G_OBJECT (self), NM_DEVICE_MASTER);
 		} else if (activating) {
 			_LOGW (LOGD_DEVICE, "Activation: connection '%s' could not be enslaved",
 			       nm_connection_get_id (connection));
 		}
 	}
 
 	if (activating) {
 		priv->ip4_state = IP_DONE;
 		priv->ip6_state = IP_DONE;
 		nm_device_queue_state (self,
 		                       success ? NM_DEVICE_STATE_SECONDARIES : NM_DEVICE_STATE_FAILED,
 		                       NM_DEVICE_STATE_REASON_NONE);
 	} else
 		nm_device_queue_recheck_assume (self);
 }
 
  
 static void
 nm_device_slave_notify_release (NMDevice *self, NMDeviceStateReason reason)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	NMConnection *connection = nm_device_get_connection (self);
 	NMDeviceState new_state;
 	const char *master_status;
 
 	if (   reason != NM_DEVICE_STATE_REASON_NONE
 	    && priv->state > NM_DEVICE_STATE_DISCONNECTED
 	    && priv->state <= NM_DEVICE_STATE_ACTIVATED) {
 		if (reason == NM_DEVICE_STATE_REASON_DEPENDENCY_FAILED) {
 			new_state = NM_DEVICE_STATE_FAILED;
 			master_status = "failed";
 		} else if (reason == NM_DEVICE_STATE_REASON_USER_REQUESTED) {
 			new_state = NM_DEVICE_STATE_DEACTIVATING;
 			master_status = "deactivated by user request";
 		} else {
 			new_state = NM_DEVICE_STATE_DISCONNECTED;
 			master_status = "deactivated";
 		}
 
 		_LOGD (LOGD_DEVICE, "Activation: connection '%s' master %s",
 		       nm_connection_get_id (connection),
 		       master_status);
 
 		nm_device_queue_state (self, new_state, reason);
 	} else if (priv->master)
 		_LOGI (LOGD_DEVICE, "released from master %s", nm_device_get_iface (priv->master));
 	else
 		_LOGD (LOGD_DEVICE, "released from master%s", priv->enslaved ? "" : " (was not enslaved)");
 
 	if (priv->enslaved) {
 		priv->enslaved = FALSE;
 		g_object_notify (G_OBJECT (self), NM_DEVICE_MASTER);
 	}
 }
 
  
 gboolean
 nm_device_get_enslaved (NMDevice *self)
 {
 	return NM_DEVICE_GET_PRIVATE (self)->enslaved;
 }
 
  
 void
 nm_device_removed (NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	if (priv->enslaved)
 		nm_device_release_one_slave (priv->master, self, FALSE, NM_DEVICE_STATE_REASON_REMOVED);
 }
 
 
 static gboolean
 is_available (NMDevice *self, NMDeviceCheckDevAvailableFlags flags)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	if (priv->carrier || priv->ignore_carrier)
 		return TRUE;
 
 	if (NM_FLAGS_HAS (flags, NM_DEVICE_CHECK_DEV_AVAILABLE_IGNORE_CARRIER))
 		return TRUE;
 
 	return FALSE;
 }
 
  
 gboolean
 nm_device_is_available (NMDevice *self, NMDeviceCheckDevAvailableFlags flags)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	if (priv->firmware_missing)
 		return FALSE;
 
 	return NM_DEVICE_GET_CLASS (self)->is_available (self, flags);
 }
 
 gboolean
 nm_device_get_enabled (NMDevice *self)
 {
 	g_return_val_if_fail (NM_IS_DEVICE (self), FALSE);
 
 	if (NM_DEVICE_GET_CLASS (self)->get_enabled)
 		return NM_DEVICE_GET_CLASS (self)->get_enabled (self);
 	return TRUE;
 }
 
 void
 nm_device_set_enabled (NMDevice *self, gboolean enabled)
 {
 	g_return_if_fail (NM_IS_DEVICE (self));
 
 	if (NM_DEVICE_GET_CLASS (self)->set_enabled)
 		NM_DEVICE_GET_CLASS (self)->set_enabled (self, enabled);
 }
 
  
 gboolean
 nm_device_get_autoconnect (NMDevice *self)
 {
 	g_return_val_if_fail (NM_IS_DEVICE (self), FALSE);
 
 	return NM_DEVICE_GET_PRIVATE (self)->autoconnect;
 }
 
 static void
 nm_device_set_autoconnect (NMDevice *self, gboolean autoconnect)
 {
 	NMDevicePrivate *priv;
 
 	g_return_if_fail (NM_IS_DEVICE (self));
 
 	priv = NM_DEVICE_GET_PRIVATE (self);
 	if (priv->autoconnect == autoconnect)
 		return;
 
 	if (autoconnect) {
 		 
 		if (!nm_device_get_default_unmanaged (self)) {
 			priv->autoconnect = TRUE;
 			g_object_notify (G_OBJECT (self), NM_DEVICE_AUTOCONNECT);
 		}
 	} else {
 		priv->autoconnect = FALSE;
 		g_object_notify (G_OBJECT (self), NM_DEVICE_AUTOCONNECT);
 	}
 }
 
 static gboolean
 autoconnect_allowed_accumulator (GSignalInvocationHint *ihint,
                                  GValue *return_accu,
                                  const GValue *handler_return, gpointer data)
 {
 	if (!g_value_get_boolean (handler_return))
 		g_value_set_boolean (return_accu, FALSE);
 	return TRUE;
 }
 
  
 gboolean
 nm_device_autoconnect_allowed (NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	GValue instance = G_VALUE_INIT;
 	GValue retval = G_VALUE_INIT;
 
 	if (priv->state < NM_DEVICE_STATE_DISCONNECTED || !priv->autoconnect)
 		return FALSE;
 
 	 
 
 	g_value_init (&instance, G_TYPE_OBJECT);
 	g_value_set_object (&instance, self);
 
 	g_value_init (&retval, G_TYPE_BOOLEAN);
 	if (priv->autoconnect)
 		g_value_set_boolean (&retval, TRUE);
 	else
 		g_value_set_boolean (&retval, FALSE);
 
 	 
 	g_signal_emitv (&instance, signals[AUTOCONNECT_ALLOWED], 0, &retval);
 	g_value_unset (&instance);
 	return g_value_get_boolean (&retval);
 }
 
 static gboolean
 can_auto_connect (NMDevice *self,
                   NMConnection *connection,
                   char **specific_object)
 {
 	NMSettingConnection *s_con;
 
 	s_con = nm_connection_get_setting_connection (connection);
 	if (!nm_setting_connection_get_autoconnect (s_con))
 		return FALSE;
 
 	return nm_device_check_connection_available (self, connection, NM_DEVICE_CHECK_CON_AVAILABLE_NONE, NULL);
 }
 
  
 gboolean
 nm_device_can_auto_connect (NMDevice *self,
                             NMConnection *connection,
                             char **specific_object)
 {
 	g_return_val_if_fail (NM_IS_DEVICE (self), FALSE);
 	g_return_val_if_fail (NM_IS_CONNECTION (connection), FALSE);
 	g_return_val_if_fail (specific_object && !*specific_object, FALSE);
 
 	if (nm_device_autoconnect_allowed (self))
 		return NM_DEVICE_GET_CLASS (self)->can_auto_connect (self, connection, specific_object);
 	return FALSE;
 }
 
 static gboolean
 device_has_config (NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	 
 	if (priv->ip4_config && nm_ip4_config_get_num_addresses (priv->ip4_config))
 		return TRUE;
 	if (priv->ip6_config && nm_ip6_config_get_num_addresses (priv->ip6_config))
 		return TRUE;
 
 	 
 	if (nm_device_is_software (self))
 		return TRUE;
 
 	 
 	if (nm_platform_link_get_master (priv->ifindex) > 0)
 		return TRUE;
 
 	return FALSE;
 }
 
  
 gboolean
 nm_device_master_update_slave_connection (NMDevice *self,
                                           NMDevice *slave,
                                           NMConnection *connection,
                                           GError **error)
 {
 	NMDeviceClass *klass;
 	gboolean success;
 
 	g_return_val_if_fail (self, FALSE);
 	g_return_val_if_fail (NM_IS_DEVICE (self), FALSE);
 	g_return_val_if_fail (slave, FALSE);
 	g_return_val_if_fail (connection, FALSE);
 	g_return_val_if_fail (!error || !*error, FALSE);
 	g_return_val_if_fail (nm_connection_get_setting_connection (connection), FALSE);
 
 	g_return_val_if_fail (nm_device_get_iface (self), FALSE);
 
 	klass = NM_DEVICE_GET_CLASS (self);
 	if (klass->master_update_slave_connection) {
 		success = klass->master_update_slave_connection (self, slave, connection, error);
 
 		g_return_val_if_fail (!error || (success && !*error) || *error, success);
 		return success;
 	}
 
 	g_set_error (error,
 	             NM_DEVICE_ERROR,
 	             NM_DEVICE_ERROR_FAILED,
 	             "master device '%s' cannot update a slave connection for slave device '%s' (master type not supported?)",
 	             nm_device_get_iface (self), nm_device_get_iface (slave));
 	return FALSE;
 }
 
 NMConnection *
 nm_device_generate_connection (NMDevice *self, NMDevice *master)
 {
 	NMDeviceClass *klass = NM_DEVICE_GET_CLASS (self);
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	const char *ifname = nm_device_get_iface (self);
 	NMConnection *connection;
 	NMSetting *s_con;
 	NMSetting *s_ip4;
 	NMSetting *s_ip6;
 	gs_free char *uuid = NULL;
 	const char *ip4_method, *ip6_method;
 	GError *error = NULL;
 
 	 
 	if (!klass->update_connection)
 		return NULL;
 
 	 
 	if (!device_has_config (self)) {
 		_LOGD (LOGD_DEVICE, "device has no existing configuration");
 		return NULL;
 	}
 
 	connection = nm_simple_connection_new ();
 	s_con = nm_setting_connection_new ();
 	uuid = nm_utils_uuid_generate ();
 
 	g_object_set (s_con,
 	              NM_SETTING_CONNECTION_UUID, uuid,
 	              NM_SETTING_CONNECTION_ID, ifname,
 	              NM_SETTING_CONNECTION_AUTOCONNECT, FALSE,
 	              NM_SETTING_CONNECTION_INTERFACE_NAME, ifname,
 	              NM_SETTING_CONNECTION_TIMESTAMP, (guint64) time (NULL),
 	              NULL);
 	if (klass->connection_type)
 		g_object_set (s_con, NM_SETTING_CONNECTION_TYPE, klass->connection_type, NULL);
 	nm_connection_add_setting (connection, s_con);
 
 	 
 	if (master) {
 		if (!nm_device_master_update_slave_connection (master,
 		                                               self,
 		                                               connection,
 		                                               &error))
 		{
 			_LOGE (LOGD_DEVICE, "master device '%s' failed to update slave connection: %s",
 			       nm_device_get_iface (master), error ? error->message : "(unknown error)");
 			g_error_free (error);
 			g_object_unref (connection);
 			return NULL;
 		}
 	} else {
 		 
 		s_ip4 = nm_ip4_config_create_setting (priv->ip4_config);
 		nm_connection_add_setting (connection, s_ip4);
 
 		s_ip6 = nm_ip6_config_create_setting (priv->ip6_config);
 		nm_connection_add_setting (connection, s_ip6);
 	}
 
 	klass->update_connection (self, connection);
 
 	 
 	if (!nm_connection_verify (connection, &error)) {
 		_LOGE (LOGD_DEVICE, "Generated connection does not verify: %s", error->message);
 		g_clear_error (&error);
 		g_object_unref (connection);
 		return NULL;
 	}
 
 	 
 	ip4_method = nm_utils_get_ip_config_method (connection, NM_TYPE_SETTING_IP4_CONFIG);
 	ip6_method = nm_utils_get_ip_config_method (connection, NM_TYPE_SETTING_IP6_CONFIG);
 	if (   g_strcmp0 (ip4_method, NM_SETTING_IP4_CONFIG_METHOD_DISABLED) == 0
 	    && g_strcmp0 (ip6_method, NM_SETTING_IP6_CONFIG_METHOD_IGNORE) == 0
 	    && !nm_setting_connection_get_master (NM_SETTING_CONNECTION (s_con))
 	    && !priv->slaves) {
 		_LOGD (LOGD_DEVICE, "ignoring generated connection (no IP and not in master-slave relationship)");
 		g_object_unref (connection);
 		connection = NULL;
 	}
 
 	return connection;
 }
 
 gboolean
 nm_device_complete_connection (NMDevice *self,
                                NMConnection *connection,
                                const char *specific_object,
                                const GSList *existing_connections,
                                GError **error)
 {
 	gboolean success = FALSE;
 
 	g_return_val_if_fail (self != NULL, FALSE);
 	g_return_val_if_fail (connection != NULL, FALSE);
 
 	if (!NM_DEVICE_GET_CLASS (self)->complete_connection) {
 		g_set_error (error, NM_DEVICE_ERROR, NM_DEVICE_ERROR_INVALID_CONNECTION,
 		             "Device class %s had no complete_connection method",
 		             G_OBJECT_TYPE_NAME (self));
 		return FALSE;
 	}
 
 	success = NM_DEVICE_GET_CLASS (self)->complete_connection (self,
 	                                                           connection,
 	                                                           specific_object,
 	                                                           existing_connections,
 	                                                           error);
 	if (success)
 		success = nm_connection_verify (connection, error);
 
 	return success;
 }
 
 static gboolean
 check_connection_compatible (NMDevice *self, NMConnection *connection)
 {
 	NMSettingConnection *s_con;
 	const char *config_iface, *device_iface;
 
 	s_con = nm_connection_get_setting_connection (connection);
 	g_assert (s_con);
 
 	config_iface = nm_setting_connection_get_interface_name (s_con);
 	device_iface = nm_device_get_iface (self);
 	if (config_iface && strcmp (config_iface, device_iface) != 0)
 		return FALSE;
 
 	return TRUE;
 }
 
  
 gboolean
 nm_device_check_connection_compatible (NMDevice *self, NMConnection *connection)
 {
 	g_return_val_if_fail (NM_IS_DEVICE (self), FALSE);
 	g_return_val_if_fail (NM_IS_CONNECTION (connection), FALSE);
 
 	return NM_DEVICE_GET_CLASS (self)->check_connection_compatible (self, connection);
 }
 
  
 static gboolean
 nm_device_can_assume_connections (NMDevice *self)
 {
 	return !!NM_DEVICE_GET_CLASS (self)->update_connection;
 }
 
  
 gboolean
 nm_device_can_assume_active_connection (NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	NMConnection *connection;
 	const char *method;
 	const char *assumable_ip6_methods[] = {
 		NM_SETTING_IP6_CONFIG_METHOD_IGNORE,
 		NM_SETTING_IP6_CONFIG_METHOD_AUTO,
 		NM_SETTING_IP6_CONFIG_METHOD_DHCP,
 		NM_SETTING_IP6_CONFIG_METHOD_LINK_LOCAL,
 		NM_SETTING_IP6_CONFIG_METHOD_MANUAL,
 		NULL
 	};
 	const char *assumable_ip4_methods[] = {
 		NM_SETTING_IP4_CONFIG_METHOD_DISABLED,
 		NM_SETTING_IP6_CONFIG_METHOD_AUTO,
 		NM_SETTING_IP6_CONFIG_METHOD_MANUAL,
 		NULL
 	};
 
 	if (!nm_device_can_assume_connections (self))
 		return FALSE;
 
 	connection = nm_device_get_connection (self);
 	if (!connection)
 		return FALSE;
 
 	 
 	if (priv->state < NM_DEVICE_STATE_IP_CONFIG)
 		return FALSE;
 	if (priv->ip4_state != IP_DONE && priv->ip6_state != IP_DONE)
 		return FALSE;
 
 	method = nm_utils_get_ip_config_method (connection, NM_TYPE_SETTING_IP6_CONFIG);
 	if (!_nm_utils_string_in_list (method, assumable_ip6_methods))
 		return FALSE;
 
 	method = nm_utils_get_ip_config_method (connection, NM_TYPE_SETTING_IP4_CONFIG);
 	if (!_nm_utils_string_in_list (method, assumable_ip4_methods))
 		return FALSE;
 
 	return TRUE;
 }
 
 static gboolean
 nm_device_emit_recheck_assume (gpointer self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	priv->recheck_assume_id = 0;
 	if (!nm_device_get_act_request (self)) {
 		_LOGD (LOGD_DEVICE, "emit RECHECK_ASSUME signal");
 		g_signal_emit (self, signals[RECHECK_ASSUME], 0);
 	}
 	return G_SOURCE_REMOVE;
 }
 
 void
 nm_device_queue_recheck_assume (NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	if (nm_device_can_assume_connections (self) && !priv->recheck_assume_id)
 		priv->recheck_assume_id = g_idle_add (nm_device_emit_recheck_assume, self);
 }
 
 void
 nm_device_emit_recheck_auto_activate (NMDevice *self)
 {
 	g_signal_emit (self, signals[RECHECK_AUTO_ACTIVATE], 0);
 }
 
 static void
 dnsmasq_state_changed_cb (NMDnsMasqManager *manager, guint32 status, gpointer user_data)
 {
 	NMDevice *self = NM_DEVICE (user_data);
 
 	switch (status) {
 	case NM_DNSMASQ_STATUS_DEAD:
 		nm_device_state_changed (self, NM_DEVICE_STATE_FAILED, NM_DEVICE_STATE_REASON_SHARED_START_FAILED);
 		break;
 	default:
 		break;
 	}
 }
 
 static void
 activation_source_clear (NMDevice *self, gboolean remove_source, int family)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	guint *act_source_id;
 	gpointer *act_source_func;
 
 	if (family == AF_INET6) {
 		act_source_id = &priv->act_source6_id;
 		act_source_func = &priv->act_source6_func;
 	} else {
 		act_source_id = &priv->act_source_id;
 		act_source_func = &priv->act_source_func;
 	}
 
 	if (*act_source_id) {
 		if (remove_source)
 			g_source_remove (*act_source_id);
 		*act_source_id = 0;
 		*act_source_func = NULL;
 	}
 }
 
 static void
 activation_source_schedule (NMDevice *self, GSourceFunc func, int family)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	guint *act_source_id;
 	gpointer *act_source_func;
 
 	if (family == AF_INET6) {
 		act_source_id = &priv->act_source6_id;
 		act_source_func = &priv->act_source6_func;
 	} else {
 		act_source_id = &priv->act_source_id;
 		act_source_func = &priv->act_source_func;
 	}
 
 	if (*act_source_id)
 		_LOGE (LOGD_DEVICE, "activation stage already scheduled");
 
 	 
 	if (!*act_source_id || (*act_source_func != func)) {
 		activation_source_clear (self, TRUE, family);
 		*act_source_id = g_idle_add (func, self);
 		*act_source_func = func;
 	}
 }
 
 static gboolean
 get_ip_config_may_fail (NMDevice *self, int family)
 {
 	NMConnection *connection;
 	NMSettingIPConfig *s_ip = NULL;
 
 	g_return_val_if_fail (self != NULL, TRUE);
 
 	connection = nm_device_get_connection (self);
 	g_assert (connection);
 
 	 
 	switch (family) {
 	case AF_INET:
 		s_ip = nm_connection_get_setting_ip4_config (connection);
 		break;
 	case AF_INET6:
 		s_ip = nm_connection_get_setting_ip6_config (connection);
 		break;
 	default:
 		g_assert_not_reached ();
 	}
 
 	return nm_setting_ip_config_get_may_fail (s_ip);
 }
 
 static void
 master_ready_cb (NMActiveConnection *active,
                  GParamSpec *pspec,
                  NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	NMActiveConnection *master;
 
 	g_assert (priv->state == NM_DEVICE_STATE_PREPARE);
 
 	 
 	g_assert (nm_active_connection_get_master_ready (active));
 	master = nm_active_connection_get_master (active);
 
 	priv->master = g_object_ref (nm_active_connection_get_device (master));
 	nm_device_master_add_slave (priv->master,
 	                            self,
 	                            nm_active_connection_get_assumed (active) ? FALSE : TRUE);
 
 	_LOGD (LOGD_DEVICE, "master connection ready; master device %s",
 	       nm_device_get_iface (priv->master));
 
 	if (priv->master_ready_id) {
 		g_signal_handler_disconnect (active, priv->master_ready_id);
 		priv->master_ready_id = 0;
 	}
 
 	nm_device_activate_schedule_stage2_device_config (self);
 }
 
 static NMActStageReturn
 act_stage1_prepare (NMDevice *self, NMDeviceStateReason *reason)
 {
 	return NM_ACT_STAGE_RETURN_SUCCESS;
 }
 
  
 static gboolean
 nm_device_activate_stage1_device_prepare (gpointer user_data)
 {
 	NMDevice *self = NM_DEVICE (user_data);
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	NMActStageReturn ret = NM_ACT_STAGE_RETURN_SUCCESS;
 	NMDeviceStateReason reason = NM_DEVICE_STATE_REASON_NONE;
 	NMActiveConnection *active = NM_ACTIVE_CONNECTION (priv->act_request);
 
 	 
 	activation_source_clear (self, FALSE, 0);
 
 	priv->ip4_state = priv->ip6_state = IP_NONE;
 
 	 
 	g_object_notify (G_OBJECT (self), NM_DEVICE_ACTIVE_CONNECTION);
 
 	_LOGI (LOGD_DEVICE, "Activation: Stage 1 of 5 (Device Prepare) started...");
 	nm_device_state_changed (self, NM_DEVICE_STATE_PREPARE, NM_DEVICE_STATE_REASON_NONE);
 
 	 
 	if (!nm_active_connection_get_assumed (active)) {
 		ret = NM_DEVICE_GET_CLASS (self)->act_stage1_prepare (self, &reason);
 		if (ret == NM_ACT_STAGE_RETURN_POSTPONE) {
 			goto out;
 		} else if (ret == NM_ACT_STAGE_RETURN_FAILURE) {
 			nm_device_state_changed (self, NM_DEVICE_STATE_FAILED, reason);
 			goto out;
 		}
 		g_assert (ret == NM_ACT_STAGE_RETURN_SUCCESS);
 	}
 
 	if (nm_active_connection_get_master (active)) {
 		 
 		if (nm_active_connection_get_master_ready (active))
 			master_ready_cb (active, NULL, self);
 		else {
 			_LOGD (LOGD_DEVICE, "waiting for master connection to become ready");
 
 			 
 			g_assert (priv->master_ready_id == 0);
 			priv->master_ready_id = g_signal_connect (active,
 			                                          "notify::" NM_ACTIVE_CONNECTION_INT_MASTER_READY,
 			                                          (GCallback) master_ready_cb,
 			                                          self);
 			 
 		}
 	} else
 		nm_device_activate_schedule_stage2_device_config (self);
 
 out:
 	_LOGI (LOGD_DEVICE, "Activation: Stage 1 of 5 (Device Prepare) complete.");
 	return FALSE;
 }
 
 
  
 void
 nm_device_activate_schedule_stage1_device_prepare (NMDevice *self)
 {
 	NMDevicePrivate *priv;
 
 	g_return_if_fail (NM_IS_DEVICE (self));
 
 	priv = NM_DEVICE_GET_PRIVATE (self);
 	g_return_if_fail (priv->act_request);
 
 	activation_source_schedule (self, nm_device_activate_stage1_device_prepare, 0);
 
 	_LOGI (LOGD_DEVICE, "Activation: Stage 1 of 5 (Device Prepare) scheduled...");
 }
 
 static NMActStageReturn
 act_stage2_config (NMDevice *self, NMDeviceStateReason *reason)
 {
 	 
 	return NM_ACT_STAGE_RETURN_SUCCESS;
 }
 
  
 static gboolean
 nm_device_activate_stage2_device_config (gpointer user_data)
 {
 	NMDevice *self = NM_DEVICE (user_data);
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	NMActStageReturn ret;
 	NMDeviceStateReason reason = NM_DEVICE_STATE_REASON_NONE;
 	gboolean no_firmware = FALSE;
 	NMActiveConnection *active = NM_ACTIVE_CONNECTION (priv->act_request);
 	GSList *iter;
 
 	 
 	activation_source_clear (self, FALSE, 0);
 
 	_LOGI (LOGD_DEVICE, "Activation: Stage 2 of 5 (Device Configure) starting...");
 	nm_device_state_changed (self, NM_DEVICE_STATE_CONFIG, NM_DEVICE_STATE_REASON_NONE);
 
 	 
 	if (!nm_active_connection_get_assumed (active)) {
 		if (!nm_device_bring_up (self, FALSE, &no_firmware)) {
 			if (no_firmware)
 				nm_device_state_changed (self, NM_DEVICE_STATE_FAILED, NM_DEVICE_STATE_REASON_FIRMWARE_MISSING);
 			else
 				nm_device_state_changed (self, NM_DEVICE_STATE_FAILED, NM_DEVICE_STATE_REASON_CONFIG_FAILED);
 			goto out;
 		}
 
 		ret = NM_DEVICE_GET_CLASS (self)->act_stage2_config (self, &reason);
 		if (ret == NM_ACT_STAGE_RETURN_POSTPONE)
 			goto out;
 		else if (ret == NM_ACT_STAGE_RETURN_FAILURE) {
 			nm_device_state_changed (self, NM_DEVICE_STATE_FAILED, reason);
 			goto out;
 		}
 		g_assert (ret == NM_ACT_STAGE_RETURN_SUCCESS);
 	}
 
 	 
 	for (iter = priv->slaves; iter; iter = g_slist_next (iter)) {
 		SlaveInfo *info = iter->data;
 		NMDeviceState slave_state = nm_device_get_state (info->slave);
 
 		if (slave_state == NM_DEVICE_STATE_IP_CONFIG)
 			nm_device_enslave_slave (self, info->slave, nm_device_get_connection (info->slave));
 		else if (   nm_device_uses_generated_assumed_connection (self)
 		         && slave_state <= NM_DEVICE_STATE_DISCONNECTED)
 			nm_device_queue_recheck_assume (info->slave);
 	}
 
 	_LOGI (LOGD_DEVICE, "Activation: Stage 2 of 5 (Device Configure) successful.");
 
 	nm_device_activate_schedule_stage3_ip_config_start (self);
 
 out:
 	_LOGI (LOGD_DEVICE, "Activation: Stage 2 of 5 (Device Configure) complete.");
 	return FALSE;
 }
 
 
  
 void
 nm_device_activate_schedule_stage2_device_config (NMDevice *self)
 {
 	NMDevicePrivate *priv;
 
 	g_return_if_fail (NM_IS_DEVICE (self));
 
 	priv = NM_DEVICE_GET_PRIVATE (self);
 	g_return_if_fail (priv->act_request);
 
 	activation_source_schedule (self, nm_device_activate_stage2_device_config, 0);
 
 	_LOGI (LOGD_DEVICE, "Activation: Stage 2 of 5 (Device Configure) scheduled...");
 }
 
  
  
 
 static void
 aipd_timeout_remove (NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	if (priv->aipd_timeout) {
 		g_source_remove (priv->aipd_timeout);
 		priv->aipd_timeout = 0;
 	}
 }
 
 static void
 aipd_cleanup (NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	if (priv->aipd_watch) {
 		g_source_remove (priv->aipd_watch);
 		priv->aipd_watch = 0;
 	}
 
 	if (priv->aipd_pid > 0) {
 		nm_utils_kill_child_sync (priv->aipd_pid, SIGKILL, LOGD_AUTOIP4, "avahi-autoipd", NULL, 0, 0);
 		priv->aipd_pid = -1;
 	}
 
 	aipd_timeout_remove (self);
 }
 
 static NMIP4Config *
 aipd_get_ip4_config (NMDevice *self, guint32 lla)
 {
 	NMIP4Config *config = NULL;
 	NMPlatformIP4Address address;
 	NMPlatformIP4Route route;
 
 	config = nm_ip4_config_new (nm_device_get_ip_ifindex (self));
 	g_assert (config);
 
 	memset (&address, 0, sizeof (address));
 	address.address = lla;
 	address.plen = 16;
 	address.source = NM_IP_CONFIG_SOURCE_IP4LL;
 	nm_ip4_config_add_address (config, &address);
 
 	 
 	memset (&route, 0, sizeof (route));
 	route.network = htonl (0xE0000000L);
 	route.plen = 4;
 	route.source = NM_IP_CONFIG_SOURCE_IP4LL;
 	route.metric = nm_device_get_ip4_route_metric (self);
 	nm_ip4_config_add_route (config, &route);
 
 	return config;
 }
 
 #define IPV4LL_NETWORK (htonl (0xA9FE0000L))
 #define IPV4LL_NETMASK (htonl (0xFFFF0000L))
 
 void
 nm_device_handle_autoip4_event (NMDevice *self,
                                 const char *event,
                                 const char *address)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	NMConnection *connection = NULL;
 	const char *method;
 	NMDeviceStateReason reason = NM_DEVICE_STATE_REASON_NONE;
 
 	g_return_if_fail (event != NULL);
 
 	if (priv->act_request == NULL)
 		return;
 
 	connection = nm_act_request_get_connection (priv->act_request);
 	g_assert (connection);
 
 	 
 	method = nm_utils_get_ip_config_method (connection, NM_TYPE_SETTING_IP4_CONFIG);
 	if (g_strcmp0 (method, NM_SETTING_IP4_CONFIG_METHOD_LINK_LOCAL) != 0)
 		return;
 
 	if (strcmp (event, "BIND") == 0) {
 		guint32 lla;
 		NMIP4Config *config;
 
 		if (inet_pton (AF_INET, address, &lla) <= 0) {
 			_LOGE (LOGD_AUTOIP4, "invalid address %s received from avahi-autoipd.", address);
 			nm_device_state_changed (self, NM_DEVICE_STATE_FAILED, NM_DEVICE_STATE_REASON_AUTOIP_ERROR);
 			return;
 		}
 
 		if ((lla & IPV4LL_NETMASK) != IPV4LL_NETWORK) {
 			_LOGE (LOGD_AUTOIP4, "invalid address %s received from avahi-autoipd (not link-local).", address);
 			nm_device_state_changed (self, NM_DEVICE_STATE_FAILED, NM_DEVICE_STATE_REASON_AUTOIP_ERROR);
 			return;
 		}
 
 		config = aipd_get_ip4_config (self, lla);
 		if (config == NULL) {
 			_LOGE (LOGD_AUTOIP4, "failed to get autoip config");
 			nm_device_state_changed (self, NM_DEVICE_STATE_FAILED, NM_DEVICE_STATE_REASON_IP_CONFIG_UNAVAILABLE);
 			return;
 		}
 
 		if (priv->ip4_state == IP_CONF) {
 			aipd_timeout_remove (self);
 			nm_device_activate_schedule_ip4_config_result (self, config);
 		} else if (priv->ip4_state == IP_DONE) {
 			if (!ip4_config_merge_and_apply (self, config, TRUE, &reason)) {
 				_LOGE (LOGD_AUTOIP4, "failed to update IP4 config for autoip change.");
 				nm_device_state_changed (self, NM_DEVICE_STATE_FAILED, reason);
 			}
 		} else
 			g_assert_not_reached ();
 
 		g_object_unref (config);
 	} else {
 		_LOGW (LOGD_AUTOIP4, "autoip address %s no longer valid because '%s'.", address, event);
 
 		 
 		nm_device_state_changed (self, NM_DEVICE_STATE_FAILED, NM_DEVICE_STATE_REASON_IP_CONFIG_EXPIRED);
 	}
 }
 
 static void
 aipd_watch_cb (GPid pid, gint status, gpointer user_data)
 {
 	NMDevice *self = NM_DEVICE (user_data);
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	NMDeviceState state;
 
 	if (!priv->aipd_watch)
 		return;
 	priv->aipd_watch = 0;
 
 	if (WIFEXITED (status))
 		_LOGD (LOGD_AUTOIP4, "avahi-autoipd exited with error code %d", WEXITSTATUS (status));
 	else if (WIFSTOPPED (status))
 		_LOGW (LOGD_AUTOIP4, "avahi-autoipd stopped unexpectedly with signal %d", WSTOPSIG (status));
 	else if (WIFSIGNALED (status))
 		_LOGW (LOGD_AUTOIP4, "avahi-autoipd died with signal %d", WTERMSIG (status));
 	else
 		_LOGW (LOGD_AUTOIP4, "avahi-autoipd died from an unknown cause");
 
 	aipd_cleanup (self);
 
 	state = nm_device_get_state (self);
 	if (nm_device_is_activating (self) || (state == NM_DEVICE_STATE_ACTIVATED))
 		nm_device_state_changed (self, NM_DEVICE_STATE_FAILED, NM_DEVICE_STATE_REASON_AUTOIP_FAILED);
 }
 
 static gboolean
 aipd_timeout_cb (gpointer user_data)
 {
 	NMDevice *self = NM_DEVICE (user_data);
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	if (priv->aipd_timeout) {
 		_LOGI (LOGD_AUTOIP4, "avahi-autoipd timed out.");
 		priv->aipd_timeout = 0;
 		aipd_cleanup (self);
 
 		if (priv->ip4_state == IP_CONF)
 			nm_device_activate_schedule_ip4_config_timeout (self);
 	}
 
 	return FALSE;
 }
 
  
 const char *nm_device_autoipd_helper_path = LIBEXECDIR "/nm-avahi-autoipd.action";
 
 static NMActStageReturn
 aipd_start (NMDevice *self, NMDeviceStateReason *reason)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	const char *argv[6];
 	char *cmdline;
 	const char *aipd_binary;
 	int i = 0;
 	GError *error = NULL;
 
 	aipd_cleanup (self);
 
 	 
 	aipd_binary = nm_utils_find_helper ("avahi-autoipd", NULL, NULL);
 	if (!aipd_binary) {
 		_LOGW (LOGD_DEVICE | LOGD_AUTOIP4,
 		       "Activation: Stage 3 of 5 (IP Configure Start) failed"
 		       " to start avahi-autoipd: not found");
 		*reason = NM_DEVICE_STATE_REASON_AUTOIP_START_FAILED;
 		return NM_ACT_STAGE_RETURN_FAILURE;
 	}
 
 	argv[i++] = aipd_binary;
 	argv[i++] = "--script";
 	argv[i++] = nm_device_autoipd_helper_path;
 
 	if (nm_logging_enabled (LOGL_DEBUG, LOGD_AUTOIP4))
 		argv[i++] = "--debug";
 	argv[i++] = nm_device_get_ip_iface (self);
 	argv[i++] = NULL;
 
 	cmdline = g_strjoinv (" ", (char **) argv);
 	_LOGD (LOGD_AUTOIP4, "running: %s", cmdline);
 	g_free (cmdline);
 
 	if (!g_spawn_async ("/", (char **) argv, NULL, G_SPAWN_DO_NOT_REAP_CHILD,
 	                    nm_utils_setpgid, NULL, &(priv->aipd_pid), &error)) {
 		_LOGW (LOGD_DEVICE | LOGD_AUTOIP4,
 		       "Activation: Stage 3 of 5 (IP Configure Start) failed"
 		       " to start avahi-autoipd: %s",
 		       error && error->message ? error->message : "(unknown)");
 		g_clear_error (&error);
 		aipd_cleanup (self);
 		return NM_ACT_STAGE_RETURN_FAILURE;
 	}
 
 	_LOGI (LOGD_DEVICE | LOGD_AUTOIP4,
 	       "Activation: Stage 3 of 5 (IP Configure Start) started"
 	       " avahi-autoipd...");
 
 	 
 	priv->aipd_watch = g_child_watch_add (priv->aipd_pid, aipd_watch_cb, self);
 
 	 
 	priv->aipd_timeout = g_timeout_add_seconds (20, aipd_timeout_cb, self);
 
 	return NM_ACT_STAGE_RETURN_POSTPONE;
 }
 
  
 
 static gboolean
 _device_get_default_route_from_platform (NMDevice *self, int addr_family, NMPlatformIPRoute *out_route)
 {
 	gboolean success = FALSE;
 	int ifindex = nm_device_get_ip_ifindex (self);
 	GArray *routes;
 
 	if (addr_family == AF_INET)
 		routes = nm_platform_ip4_route_get_all (ifindex, NM_PLATFORM_GET_ROUTE_MODE_ONLY_DEFAULT);
 	else
 		routes = nm_platform_ip6_route_get_all (ifindex, NM_PLATFORM_GET_ROUTE_MODE_ONLY_DEFAULT);
 
 	if (routes) {
 		guint route_metric = G_MAXUINT32, m;
 		const NMPlatformIPRoute *route = NULL, *r;
 		guint i;
 
 		 
 		for (i = 0; i < routes->len; i++) {
 			if (addr_family == AF_INET) {
 				r = (const NMPlatformIPRoute *) &g_array_index (routes, NMPlatformIP4Route, i);
 				m = r->metric;
 			} else {
 				r = (const NMPlatformIPRoute *) &g_array_index (routes, NMPlatformIP6Route, i);
 				m = nm_utils_ip6_route_metric_normalize (r->metric);
 			}
 			if (!route || m < route_metric) {
 				route = r;
 				route_metric = m;
 			}
 		}
 
 		if (route) {
 			if (addr_family == AF_INET)
 				*((NMPlatformIP4Route *) out_route) = *((NMPlatformIP4Route *) route);
 			else
 				*((NMPlatformIP6Route *) out_route) = *((NMPlatformIP6Route *) route);
 			success = TRUE;
 		}
 		g_array_free (routes, TRUE);
 	}
 	return success;
 }
 
  
 
 static void
 ensure_con_ipx_config (NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	int ip_ifindex = nm_device_get_ip_ifindex (self);
 	NMConnection *connection;
 
 	g_assert (!!priv->con_ip4_config == !!priv->con_ip6_config);
 
 	if (priv->con_ip4_config)
 		return;
 
 	connection = nm_device_get_connection (self);
 	if (!connection)
 		return;
 
 	priv->con_ip4_config = nm_ip4_config_new (ip_ifindex);
 	priv->con_ip6_config = nm_ip6_config_new (ip_ifindex);
 
 	nm_ip4_config_merge_setting (priv->con_ip4_config,
 	                             nm_connection_get_setting_ip4_config (connection),
 	                             nm_device_get_ip4_route_metric (self));
 	nm_ip6_config_merge_setting (priv->con_ip6_config,
 	                             nm_connection_get_setting_ip6_config (connection),
 	                             nm_device_get_ip6_route_metric (self));
 
 	if (nm_device_uses_assumed_connection (self)) {
 		 
 		nm_ip4_config_reset_addresses (priv->con_ip4_config);
 		nm_ip4_config_reset_routes (priv->con_ip4_config);
 
 		nm_ip6_config_reset_addresses (priv->con_ip6_config);
 		nm_ip6_config_reset_routes (priv->con_ip6_config);
 	}
 }
 
  
  
 
 static void
 dhcp4_cleanup (NMDevice *self, gboolean stop, gboolean release)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	if (priv->dhcp4_client) {
 		 
 		if (priv->dhcp4_state_sigid) {
 			g_signal_handler_disconnect (priv->dhcp4_client, priv->dhcp4_state_sigid);
 			priv->dhcp4_state_sigid = 0;
 		}
 
 		nm_device_remove_pending_action (self, PENDING_ACTION_DHCP4, FALSE);
 
 		if (stop)
 			nm_dhcp_client_stop (priv->dhcp4_client, release);
 
 		g_clear_object (&priv->dhcp4_client);
 	}
 
 	if (priv->dhcp4_config) {
 		g_clear_object (&priv->dhcp4_config);
 		g_object_notify (G_OBJECT (self), NM_DEVICE_DHCP4_CONFIG);
 	}
 }
 
 static gboolean
 ip4_config_merge_and_apply (NMDevice *self,
                             NMIP4Config *config,
                             gboolean commit,
                             NMDeviceStateReason *out_reason)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	NMConnection *connection;
 	gboolean success;
 	NMIP4Config *composite;
 	gboolean has_direct_route;
 	const guint32 default_route_metric = nm_device_get_ip4_route_metric (self);
 	guint32 gateway;
 
 	 
 	if (config) {
 		g_clear_object (&priv->dev_ip4_config);
 		priv->dev_ip4_config = g_object_ref (config);
 	}
 
 	composite = nm_ip4_config_new (nm_device_get_ip_ifindex (self));
 
 	ensure_con_ipx_config (self);
 
 	if (priv->dev_ip4_config)
 		nm_ip4_config_merge (composite, priv->dev_ip4_config);
 	if (priv->vpn4_config)
 		nm_ip4_config_merge (composite, priv->vpn4_config);
 	if (priv->ext_ip4_config)
 		nm_ip4_config_merge (composite, priv->ext_ip4_config);
 
 	 
 	if (priv->wwan_ip4_config)
 		nm_ip4_config_merge (composite, priv->wwan_ip4_config);
 
 	 
 	if (priv->con_ip4_config)
 		nm_ip4_config_merge (composite, priv->con_ip4_config);
 
 	connection = nm_device_get_connection (self);
 
 	 
 
 	 
 	priv->default_route.v4_has = FALSE;
 	priv->default_route.v4_is_assumed = TRUE;
 
 	if (!commit) {
 		 
 		goto END_ADD_DEFAULT_ROUTE;
 	}
 
 	if (nm_device_uses_assumed_connection (self))
 		goto END_ADD_DEFAULT_ROUTE;
 
 
 	 
 	priv->default_route.v4_is_assumed = FALSE;
 
 	if (   !connection
 	    || !nm_default_route_manager_ip4_connection_has_default_route (nm_default_route_manager_get (), connection))
 		goto END_ADD_DEFAULT_ROUTE;
 
 	if (!nm_ip4_config_get_num_addresses (composite)) {
 		 
 		goto END_ADD_DEFAULT_ROUTE;
 	}
 
 	gateway = nm_ip4_config_get_gateway (composite);
 	if (   !gateway
 	    && nm_device_get_device_type (self) != NM_DEVICE_TYPE_MODEM)
 		goto END_ADD_DEFAULT_ROUTE;
 
 	has_direct_route = (   gateway == 0
 	                    || nm_ip4_config_get_subnet_for_host (composite, gateway)
 	                    || nm_ip4_config_get_direct_route_for_host (composite, gateway));
 
 	priv->default_route.v4_has = TRUE;
 	memset (&priv->default_route.v4, 0, sizeof (priv->default_route.v4));
 	priv->default_route.v4.source = NM_IP_CONFIG_SOURCE_USER;
 	priv->default_route.v4.gateway = gateway;
 	priv->default_route.v4.metric = default_route_metric;
 	priv->default_route.v4.mss = nm_ip4_config_get_mss (composite);
 
 	if (!has_direct_route) {
 		NMPlatformIP4Route r = priv->default_route.v4;
 
 		 
 		r.network = gateway;
 		r.plen = 32;
 		r.gateway = 0;
 		nm_ip4_config_add_route (composite, &r);
 	}
 
 END_ADD_DEFAULT_ROUTE:
 
 	if (priv->default_route.v4_is_assumed) {
 		 
 		priv->default_route.v4_has = _device_get_default_route_from_platform (self, AF_INET, (NMPlatformIPRoute *) &priv->default_route.v4);
 	}
 
 	 
 	if (commit) {
 		if (NM_DEVICE_GET_CLASS (self)->ip4_config_pre_commit)
 			NM_DEVICE_GET_CLASS (self)->ip4_config_pre_commit (self, composite);
 	}
 
 	success = nm_device_set_ip4_config (self, composite, default_route_metric, commit, out_reason);
 	g_object_unref (composite);
 	return success;
 }
 
 static void
 dhcp4_lease_change (NMDevice *self, NMIP4Config *config)
 {
 	NMDeviceStateReason reason = NM_DEVICE_STATE_REASON_NONE;
 
 	g_return_if_fail (config != NULL);
 
 	if (!ip4_config_merge_and_apply (self, config, TRUE, &reason)) {
 		_LOGW (LOGD_DHCP4, "failed to update IPv4 config for DHCP change.");
 		nm_device_state_changed (self, NM_DEVICE_STATE_FAILED, reason);
 	} else {
 		 
 		nm_dispatcher_call (DISPATCHER_ACTION_DHCP4_CHANGE,
 		                    nm_device_get_connection (self),
 		                    self,
 		                    NULL,
 		                    NULL,
 		                    NULL);
 	}
 }
 
 static void
 dhcp4_fail (NMDevice *self, gboolean timeout)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	dhcp4_cleanup (self, TRUE, FALSE);
 	if (timeout || (priv->ip4_state == IP_CONF))
 		nm_device_activate_schedule_ip4_config_timeout (self);
 	else if (priv->ip4_state == IP_DONE)
 		nm_device_state_changed (self, NM_DEVICE_STATE_FAILED, NM_DEVICE_STATE_REASON_IP_CONFIG_EXPIRED);
 	else
 		g_warn_if_reached ();
 }
 
 static void
 dhcp4_update_config (NMDevice *self, NMDhcp4Config *config, GHashTable *options)
 {
 	GHashTableIter iter;
 	const char *key, *value;
 
 	 
 	nm_dhcp4_config_reset (config);
 
 	g_hash_table_iter_init (&iter, options);
 	while (g_hash_table_iter_next (&iter, (gpointer) &key, (gpointer) &value))
 		nm_dhcp4_config_add_option (config, key, value);
 
 	g_object_notify (G_OBJECT (self), NM_DEVICE_DHCP4_CONFIG);
 }
 
 static void
 dhcp4_state_changed (NMDhcpClient *client,
                      NMDhcpState state,
                      NMIP4Config *ip4_config,
                      GHashTable *options,
                      gpointer user_data)
 {
 	NMDevice *self = NM_DEVICE (user_data);
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	g_return_if_fail (nm_dhcp_client_get_ipv6 (client) == FALSE);
 	g_return_if_fail (!ip4_config || NM_IS_IP4_CONFIG (ip4_config));
 
 	_LOGD (LOGD_DHCP4, "new DHCPv4 client state %d", state);
 
 	switch (state) {
 	case NM_DHCP_STATE_BOUND:
 		if (!ip4_config) {
 			_LOGW (LOGD_DHCP4, "failed to get IPv4 config in response to DHCP event.");
 			nm_device_state_changed (self,
 			                         NM_DEVICE_STATE_FAILED,
 			                         NM_DEVICE_STATE_REASON_IP_CONFIG_UNAVAILABLE);
 			break;
 		}
 
 		dhcp4_update_config (self, priv->dhcp4_config, options);
 
 		if (priv->ip4_state == IP_CONF)
 			nm_device_activate_schedule_ip4_config_result (self, ip4_config);
 		else if (priv->ip4_state == IP_DONE)
 			dhcp4_lease_change (self, ip4_config);
 		break;
 	case NM_DHCP_STATE_TIMEOUT:
 		dhcp4_fail (self, TRUE);
 		break;
 	case NM_DHCP_STATE_EXPIRE:
 		 
 		if (priv->ip4_state == IP_CONF)
 			break;
 		 
 	case NM_DHCP_STATE_DONE:
 	case NM_DHCP_STATE_FAIL:
 		dhcp4_fail (self, FALSE);
 		break;
 	default:
 		break;
 	}
 }
 
 static NMActStageReturn
 dhcp4_start (NMDevice *self,
              NMConnection *connection,
              NMDeviceStateReason *reason)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	NMSettingIPConfig *s_ip4;
 	const guint8 *hw_addr;
 	size_t hw_addr_len = 0;
 	GByteArray *tmp = NULL;
 
 	s_ip4 = nm_connection_get_setting_ip4_config (connection);
 
 	 
 	if (priv->dhcp4_config)
 		g_object_unref (priv->dhcp4_config);
 	priv->dhcp4_config = nm_dhcp4_config_new ();
 
 	hw_addr = nm_platform_link_get_address (nm_device_get_ip_ifindex (self), &hw_addr_len);
 	if (hw_addr_len) {
 		tmp = g_byte_array_sized_new (hw_addr_len);
 		g_byte_array_append (tmp, hw_addr, hw_addr_len);
 	}
 
 	 
 	g_warn_if_fail (priv->dhcp4_client == NULL);
 	priv->dhcp4_client = nm_dhcp_manager_start_ip4 (nm_dhcp_manager_get (),
 	                                                nm_device_get_ip_iface (self),
 	                                                nm_device_get_ip_ifindex (self),
 	                                                tmp,
 	                                                nm_connection_get_uuid (connection),
 	                                                nm_device_get_ip4_route_metric (self),
 	                                                nm_setting_ip_config_get_dhcp_send_hostname (s_ip4),
 	                                                nm_setting_ip_config_get_dhcp_hostname (s_ip4),
 	                                                nm_setting_ip4_config_get_dhcp_client_id (NM_SETTING_IP4_CONFIG (s_ip4)),
 	                                                priv->dhcp_timeout,
 	                                                priv->dhcp_anycast_address,
 	                                                NULL);
 
 	if (tmp)
 		g_byte_array_free (tmp, TRUE);
 
 	if (!priv->dhcp4_client) {
 		*reason = NM_DEVICE_STATE_REASON_DHCP_START_FAILED;
 		return NM_ACT_STAGE_RETURN_FAILURE;
 	}
 
 	priv->dhcp4_state_sigid = g_signal_connect (priv->dhcp4_client,
 	                                            NM_DHCP_CLIENT_SIGNAL_STATE_CHANGED,
 	                                            G_CALLBACK (dhcp4_state_changed),
 	                                            self);
 
 	nm_device_add_pending_action (self, PENDING_ACTION_DHCP4, TRUE);
 
 	 
 	return NM_ACT_STAGE_RETURN_POSTPONE;
 }
 
 gboolean
 nm_device_dhcp4_renew (NMDevice *self, gboolean release)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	NMActStageReturn ret;
 	NMDeviceStateReason reason;
 	NMConnection *connection;
 
 	g_return_val_if_fail (priv->dhcp4_client != NULL, FALSE);
 
 	_LOGI (LOGD_DHCP4, "DHCPv4 lease renewal requested");
 
 	 
 	dhcp4_cleanup (self, TRUE, release);
 
 	connection = nm_device_get_connection (self);
 	g_assert (connection);
 
 	 
 	ret = dhcp4_start (self, connection, &reason);
 
 	return (ret != NM_ACT_STAGE_RETURN_FAILURE);
 }
 
  
 
 static GHashTable *shared_ips = NULL;
 
 static void
 release_shared_ip (gpointer data)
 {
 	g_hash_table_remove (shared_ips, data);
 }
 
 static gboolean
 reserve_shared_ip (NMDevice *self, NMSettingIPConfig *s_ip4, NMPlatformIP4Address *address)
 {
 	if (G_UNLIKELY (shared_ips == NULL))
 		shared_ips = g_hash_table_new (g_direct_hash, g_direct_equal);
 
 	memset (address, 0, sizeof (*address));
 
 	if (s_ip4 && nm_setting_ip_config_get_num_addresses (s_ip4)) {
 		 
 		NMIPAddress *user = nm_setting_ip_config_get_address (s_ip4, 0);
 
 		g_assert (user);
 		nm_ip_address_get_address_binary (user, &address->address);
 		address->plen = nm_ip_address_get_prefix (user);
 	} else {
 		 
 		guint32 start = (guint32) ntohl (0x0a2a0001);  
 		guint32 count = 0;
 
 		while (g_hash_table_lookup (shared_ips, GUINT_TO_POINTER (start + count))) {
 			count += ntohl (0x100);
 			if (count > ntohl (0xFE00)) {
 				_LOGE (LOGD_SHARING, "ran out of shared IP addresses!");
 				return FALSE;
 			}
 		}
 		address->address = start + count;
 		address->plen = 24;
 
 		g_hash_table_insert (shared_ips,
 		                     GUINT_TO_POINTER (address->address),
 		                     GUINT_TO_POINTER (TRUE));
 	}
 
 	return TRUE;
 }
 
 static NMIP4Config *
 shared4_new_config (NMDevice *self, NMConnection *connection, NMDeviceStateReason *reason)
 {
 	NMIP4Config *config = NULL;
 	NMPlatformIP4Address address;
 
 	g_return_val_if_fail (self != NULL, NULL);
 
 	if (!reserve_shared_ip (self, nm_connection_get_setting_ip4_config (connection), &address)) {
 		*reason = NM_DEVICE_STATE_REASON_IP_CONFIG_UNAVAILABLE;
 		return NULL;
 	}
 
 	config = nm_ip4_config_new (nm_device_get_ip_ifindex (self));
 	address.source = NM_IP_CONFIG_SOURCE_SHARED;
 	nm_ip4_config_add_address (config, &address);
 
 	 
 	g_object_set_data_full (G_OBJECT (config), "shared-ip",
 	                        GUINT_TO_POINTER (address.address),
 	                        release_shared_ip);
 
 	return config;
 }
 
  
 
 static gboolean
 connection_ip4_method_requires_carrier (NMConnection *connection,
                                         gboolean *out_ip4_enabled)
 {
 	const char *method = nm_utils_get_ip_config_method (connection, NM_TYPE_SETTING_IP4_CONFIG);
 	static const char *ip4_carrier_methods[] = {
 		NM_SETTING_IP4_CONFIG_METHOD_AUTO,
 		NM_SETTING_IP4_CONFIG_METHOD_LINK_LOCAL,
 		NULL
 	};
 
 	if (out_ip4_enabled)
 		*out_ip4_enabled = !!strcmp (method, NM_SETTING_IP4_CONFIG_METHOD_DISABLED);
 	return _nm_utils_string_in_list (method, ip4_carrier_methods);
 }
 
 static gboolean
 connection_ip6_method_requires_carrier (NMConnection *connection,
                                         gboolean *out_ip6_enabled)
 {
 	const char *method = nm_utils_get_ip_config_method (connection, NM_TYPE_SETTING_IP6_CONFIG);
 	static const char *ip6_carrier_methods[] = {
 		NM_SETTING_IP6_CONFIG_METHOD_AUTO,
 		NM_SETTING_IP6_CONFIG_METHOD_DHCP,
 		NM_SETTING_IP6_CONFIG_METHOD_LINK_LOCAL,
 		NULL
 	};
 
 	if (out_ip6_enabled)
 		*out_ip6_enabled = !!strcmp (method, NM_SETTING_IP6_CONFIG_METHOD_IGNORE);
 	return _nm_utils_string_in_list (method, ip6_carrier_methods);
 }
 
 static gboolean
 connection_requires_carrier (NMConnection *connection)
 {
 	NMSettingIPConfig *s_ip4, *s_ip6;
 	gboolean ip4_carrier_wanted, ip6_carrier_wanted;
 	gboolean ip4_used = FALSE, ip6_used = FALSE;
 
 	ip4_carrier_wanted = connection_ip4_method_requires_carrier (connection, &ip4_used);
 	if (ip4_carrier_wanted) {
 		 
 		s_ip4 = nm_connection_get_setting_ip4_config (connection);
 		if (s_ip4 && !nm_setting_ip_config_get_may_fail (s_ip4))
 			return TRUE;
 	}
 
 	ip6_carrier_wanted = connection_ip6_method_requires_carrier (connection, &ip6_used);
 	if (ip6_carrier_wanted) {
 		 
 		s_ip6 = nm_connection_get_setting_ip6_config (connection);
 		if (s_ip6 && !nm_setting_ip_config_get_may_fail (s_ip6))
 			return TRUE;
 	}
 
 	 
 	if (ip4_carrier_wanted && !ip6_used)
 		return TRUE;
 	if (ip6_carrier_wanted && !ip4_used)
 		return TRUE;
 
 	 
 	return ip4_carrier_wanted && ip6_carrier_wanted;
 }
 
 static gboolean
 have_any_ready_slaves (NMDevice *self, const GSList *slaves)
 {
 	const GSList *iter;
 
 	 
 	for (iter = slaves; iter; iter = g_slist_next (iter)) {
 		if (nm_device_get_enslaved (iter->data))
 			return TRUE;
 	}
 	return FALSE;
 }
 
 static gboolean
 ip4_requires_slaves (NMConnection *connection)
 {
 	const char *method;
 
 	method = nm_utils_get_ip_config_method (connection, NM_TYPE_SETTING_IP4_CONFIG);
 	return strcmp (method, NM_SETTING_IP4_CONFIG_METHOD_AUTO) == 0;
 }
 
 static NMActStageReturn
 act_stage3_ip4_config_start (NMDevice *self,
                              NMIP4Config **out_config,
                              NMDeviceStateReason *reason)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	NMConnection *connection;
 	NMActStageReturn ret = NM_ACT_STAGE_RETURN_FAILURE;
 	const char *method;
 	GSList *slaves;
 	gboolean ready_slaves;
 
 	g_return_val_if_fail (reason != NULL, NM_ACT_STAGE_RETURN_FAILURE);
 
 	connection = nm_device_get_connection (self);
 	g_assert (connection);
 
 	if (   connection_ip4_method_requires_carrier (connection, NULL)
 	    && priv->is_master
 	    && !priv->carrier) {
 		_LOGI (LOGD_IP4 | LOGD_DEVICE,
 		       "IPv4 config waiting until carrier is on");
 		return NM_ACT_STAGE_RETURN_WAIT;
 	}
 
 	if (priv->is_master && ip4_requires_slaves (connection)) {
 		 
 		slaves = nm_device_master_get_slaves (self);
 		ready_slaves = NM_DEVICE_GET_CLASS (self)->have_any_ready_slaves (self, slaves);
 		g_slist_free (slaves);
 
 		if (ready_slaves == FALSE) {
 			_LOGI (LOGD_DEVICE | LOGD_IP4,
 			       "IPv4 config waiting until slaves are ready");
 			return NM_ACT_STAGE_RETURN_WAIT;
 		}
 	}
 
 	method = nm_utils_get_ip_config_method (connection, NM_TYPE_SETTING_IP4_CONFIG);
 
 	 
 	if (strcmp (method, NM_SETTING_IP4_CONFIG_METHOD_AUTO) == 0)
 		ret = dhcp4_start (self, connection, reason);
 	else if (strcmp (method, NM_SETTING_IP4_CONFIG_METHOD_LINK_LOCAL) == 0)
 		ret = aipd_start (self, reason);
 	else if (strcmp (method, NM_SETTING_IP4_CONFIG_METHOD_MANUAL) == 0) {
 		 
 		*out_config = nm_ip4_config_new (nm_device_get_ip_ifindex (self));
 		g_assert (*out_config);
 		ret = NM_ACT_STAGE_RETURN_SUCCESS;
 	} else if (strcmp (method, NM_SETTING_IP4_CONFIG_METHOD_SHARED) == 0) {
 		*out_config = shared4_new_config (self, connection, reason);
 		if (*out_config) {
 			priv->dnsmasq_manager = nm_dnsmasq_manager_new (nm_device_get_ip_iface (self));
 			ret = NM_ACT_STAGE_RETURN_SUCCESS;
 		} else
 			ret = NM_ACT_STAGE_RETURN_FAILURE;
 	} else if (strcmp (method, NM_SETTING_IP4_CONFIG_METHOD_DISABLED) == 0) {
 		 
 		ret = NM_ACT_STAGE_RETURN_STOP;
 	} else
 		_LOGW (LOGD_IP4, "unhandled IPv4 config method '%s'; will fail", method);
 
 	return ret;
 }
 
  
  
 
 static void
 dhcp6_cleanup (NMDevice *self, gboolean stop, gboolean release)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	priv->dhcp6_mode = NM_RDISC_DHCP_LEVEL_NONE;
 	g_clear_object (&priv->dhcp6_ip6_config);
 
 	if (priv->dhcp6_client) {
 		if (priv->dhcp6_state_sigid) {
 			g_signal_handler_disconnect (priv->dhcp6_client, priv->dhcp6_state_sigid);
 			priv->dhcp6_state_sigid = 0;
 		}
 
 		if (stop)
 			nm_dhcp_client_stop (priv->dhcp6_client, release);
 
 		g_clear_object (&priv->dhcp6_client);
 	}
 
 	nm_device_remove_pending_action (self, PENDING_ACTION_DHCP6, FALSE);
 
 	if (priv->dhcp6_config) {
 		g_clear_object (&priv->dhcp6_config);
 		g_object_notify (G_OBJECT (self), NM_DEVICE_DHCP6_CONFIG);
 	}
 }
 
 static gboolean
 ip6_config_merge_and_apply (NMDevice *self,
                             gboolean commit,
                             NMDeviceStateReason *out_reason)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	NMConnection *connection;
 	gboolean success;
 	NMIP6Config *composite;
 	gboolean has_direct_route;
 	const struct in6_addr *gateway;
 
 	 
 	composite = nm_ip6_config_new (nm_device_get_ip_ifindex (self));
 
 	ensure_con_ipx_config (self);
 	g_assert (composite);
 
 	 
 	if (priv->ac_ip6_config)
 		nm_ip6_config_merge (composite, priv->ac_ip6_config);
 	if (priv->dhcp6_ip6_config)
 		nm_ip6_config_merge (composite, priv->dhcp6_ip6_config);
 	if (priv->vpn6_config)
 		nm_ip6_config_merge (composite, priv->vpn6_config);
 	if (priv->ext_ip6_config)
 		nm_ip6_config_merge (composite, priv->ext_ip6_config);
 
 	 
 	if (priv->wwan_ip6_config)
 		nm_ip6_config_merge (composite, priv->wwan_ip6_config);
 
 	 
 	if (priv->con_ip6_config)
 		nm_ip6_config_merge (composite, priv->con_ip6_config);
 
 	connection = nm_device_get_connection (self);
 
 	 
 
 	 
 	priv->default_route.v6_has = FALSE;
 	priv->default_route.v6_is_assumed = TRUE;
 
 	if (!commit) {
 		 
 		goto END_ADD_DEFAULT_ROUTE;
 	}
 
 	if (nm_device_uses_assumed_connection (self))
 		goto END_ADD_DEFAULT_ROUTE;
 
 
 	 
 	priv->default_route.v6_is_assumed = FALSE;
 
 	if (   !connection
 	    || !nm_default_route_manager_ip6_connection_has_default_route (nm_default_route_manager_get (), connection))
 		goto END_ADD_DEFAULT_ROUTE;
 
 	if (!nm_ip6_config_get_num_addresses (composite)) {
 		 
 		goto END_ADD_DEFAULT_ROUTE;
 	}
 
 	gateway = nm_ip6_config_get_gateway (composite);
 	if (!gateway)
 		goto END_ADD_DEFAULT_ROUTE;
 
 
 	has_direct_route = nm_ip6_config_get_direct_route_for_host (composite, gateway) != NULL;
 
 
 
 	priv->default_route.v6_has = TRUE;
 	memset (&priv->default_route.v6, 0, sizeof (priv->default_route.v6));
 	priv->default_route.v6.source = NM_IP_CONFIG_SOURCE_USER;
 	priv->default_route.v6.gateway = *gateway;
 	priv->default_route.v6.metric = nm_device_get_ip6_route_metric (self);
 	priv->default_route.v6.mss = nm_ip6_config_get_mss (composite);
 
 	if (!has_direct_route) {
 		NMPlatformIP6Route r = priv->default_route.v6;
 
 		 
 		r.network = *gateway;
 		r.plen = 128;
 		r.gateway = in6addr_any;
 		nm_ip6_config_add_route (composite, &r);
 	}
 
 END_ADD_DEFAULT_ROUTE:
 
 	if (priv->default_route.v6_is_assumed) {
 		 
 		priv->default_route.v6_has = _device_get_default_route_from_platform (self, AF_INET6, (NMPlatformIPRoute *) &priv->default_route.v6);
 	}
 
 	nm_ip6_config_addresses_sort (composite,
 	    priv->rdisc ? priv->rdisc_use_tempaddr : NM_SETTING_IP6_CONFIG_PRIVACY_UNKNOWN);
 
 	 
 	if (commit) {
 		if (NM_DEVICE_GET_CLASS (self)->ip6_config_pre_commit)
 			NM_DEVICE_GET_CLASS (self)->ip6_config_pre_commit (self, composite);
 	}
 
 	success = nm_device_set_ip6_config (self, composite, commit, out_reason);
 	g_object_unref (composite);
 	return success;
 }
 
 static void
 dhcp6_lease_change (NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	NMConnection *connection;
 	NMDeviceStateReason reason = NM_DEVICE_STATE_REASON_NONE;
 
 	if (priv->dhcp6_ip6_config == NULL) {
 		_LOGW (LOGD_DHCP6, "failed to get DHCPv6 config for rebind");
 		nm_device_state_changed (self, NM_DEVICE_STATE_FAILED, NM_DEVICE_STATE_REASON_IP_CONFIG_EXPIRED);
 		return;
 	}
 
 	g_assert (priv->dhcp6_client);   
 
 	connection = nm_device_get_connection (self);
 	g_assert (connection);
 
 	 
 	if (ip6_config_merge_and_apply (self, TRUE, &reason) == FALSE) {
 		_LOGW (LOGD_DHCP6, "failed to update IPv6 config in response to DHCP event.");
 		nm_device_state_changed (self, NM_DEVICE_STATE_FAILED, reason);
 	} else {
 		 
 		nm_dispatcher_call (DISPATCHER_ACTION_DHCP6_CHANGE, connection, self, NULL, NULL, NULL);
 	}
 }
 
 static void
 dhcp6_fail (NMDevice *self, gboolean timeout)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	dhcp6_cleanup (self, TRUE, FALSE);
 
 	if (priv->dhcp6_mode == NM_RDISC_DHCP_LEVEL_MANAGED) {
 		if (timeout || (priv->ip6_state == IP_CONF))
 			nm_device_activate_schedule_ip6_config_timeout (self);
 		else if (priv->ip6_state == IP_DONE)
 			nm_device_state_changed (self, NM_DEVICE_STATE_FAILED, NM_DEVICE_STATE_REASON_IP_CONFIG_EXPIRED);
 		else
 			g_warn_if_reached ();
 	} else {
 		 
 		if (priv->ip6_state == IP_CONF)
 			nm_device_activate_schedule_ip6_config_result (self);
 	}
 }
 
 static void
 dhcp6_timeout (NMDevice *self, NMDhcpClient *client)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	if (priv->dhcp6_mode == NM_RDISC_DHCP_LEVEL_MANAGED)
 		dhcp6_fail (self, TRUE);
 	else {
 		 
 		dhcp6_cleanup (self, TRUE, FALSE);
 		if (priv->ip6_state == IP_CONF)
 			nm_device_activate_schedule_ip6_config_result (self);
 	}
 }
 
 static void
 dhcp6_update_config (NMDevice *self, NMDhcp6Config *config, GHashTable *options)
 {
 	GHashTableIter iter;
 	const char *key, *value;
 
 	 
 	nm_dhcp6_config_reset (config);
 
 	g_hash_table_iter_init (&iter, options);
 	while (g_hash_table_iter_next (&iter, (gpointer) &key, (gpointer) &value))
 		nm_dhcp6_config_add_option (config, key, value);
 
 	g_object_notify (G_OBJECT (self), NM_DEVICE_DHCP6_CONFIG);
 }
 
 static void
 dhcp6_state_changed (NMDhcpClient *client,
                      NMDhcpState state,
                      NMIP6Config *ip6_config,
                      GHashTable *options,
                      gpointer user_data)
 {
 	NMDevice *self = NM_DEVICE (user_data);
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	g_return_if_fail (nm_dhcp_client_get_ipv6 (client) == TRUE);
 	g_return_if_fail (!ip6_config || NM_IS_IP6_CONFIG (ip6_config));
 
 	_LOGD (LOGD_DHCP6, "new DHCPv6 client state %d", state);
 
 	switch (state) {
 	case NM_DHCP_STATE_BOUND:
 		g_clear_object (&priv->dhcp6_ip6_config);
 		if (ip6_config) {
 			priv->dhcp6_ip6_config = g_object_ref (ip6_config);
 			dhcp6_update_config (self, priv->dhcp6_config, options);
 		}
 
 		if (priv->ip6_state == IP_CONF) {
 			if (priv->dhcp6_ip6_config == NULL) {
 				 
 				nm_device_state_changed (self, NM_DEVICE_STATE_FAILED, NM_DEVICE_STATE_REASON_DHCP_FAILED);
 				break;
 			}
 			nm_device_activate_schedule_ip6_config_result (self);
 		} else if (priv->ip6_state == IP_DONE)
 			dhcp6_lease_change (self);
 		break;
 	case NM_DHCP_STATE_TIMEOUT:
 		dhcp6_timeout (self, client);
 		break;
 	case NM_DHCP_STATE_EXPIRE:
 		 
 		if (priv->ip6_state != IP_CONF)
 			dhcp6_fail (self, FALSE);
 		break;
 	case NM_DHCP_STATE_DONE:
 		 
 		if (priv->dhcp6_mode == NM_RDISC_DHCP_LEVEL_OTHERCONF)
 			break;
 		 
 	case NM_DHCP_STATE_FAIL:
 		dhcp6_fail (self, FALSE);
 		break;
 	default:
 		break;
 	}
 }
 
 static gboolean
 dhcp6_start_with_link_ready (NMDevice *self, NMConnection *connection)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	NMSettingIPConfig *s_ip6;
 	GByteArray *tmp = NULL;
 	const guint8 *hw_addr;
 	size_t hw_addr_len = 0;
 
 	g_assert (connection);
 	s_ip6 = nm_connection_get_setting_ip6_config (connection);
 	g_assert (s_ip6);
 
 	hw_addr = nm_platform_link_get_address (nm_device_get_ip_ifindex (self), &hw_addr_len);
 	if (hw_addr_len) {
 		tmp = g_byte_array_sized_new (hw_addr_len);
 		g_byte_array_append (tmp, hw_addr, hw_addr_len);
 	}
 
 	priv->dhcp6_client = nm_dhcp_manager_start_ip6 (nm_dhcp_manager_get (),
 	                                                nm_device_get_ip_iface (self),
 	                                                nm_device_get_ip_ifindex (self),
 	                                                tmp,
 	                                                nm_connection_get_uuid (connection),
 	                                                nm_device_get_ip6_route_metric (self),
 	                                                nm_setting_ip_config_get_dhcp_send_hostname (s_ip6),
 	                                                nm_setting_ip_config_get_dhcp_hostname (s_ip6),
 	                                                priv->dhcp_timeout,
 	                                                priv->dhcp_anycast_address,
 	                                                (priv->dhcp6_mode == NM_RDISC_DHCP_LEVEL_OTHERCONF) ? TRUE : FALSE,
 	                                                nm_setting_ip6_config_get_ip6_privacy (NM_SETTING_IP6_CONFIG (s_ip6)));
 	if (tmp)
 		g_byte_array_free (tmp, TRUE);
 
 	if (priv->dhcp6_client) {
 		priv->dhcp6_state_sigid = g_signal_connect (priv->dhcp6_client,
 		                                            NM_DHCP_CLIENT_SIGNAL_STATE_CHANGED,
 		                                            G_CALLBACK (dhcp6_state_changed),
 		                                            self);
 	}
 
 	return !!priv->dhcp6_client;
 }
 
 static gboolean
 dhcp6_start (NMDevice *self, gboolean wait_for_ll, NMDeviceStateReason *reason)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	NMConnection *connection;
 	NMSettingIPConfig *s_ip6;
 
 	g_clear_object (&priv->dhcp6_config);
 	priv->dhcp6_config = nm_dhcp6_config_new ();
 
 	g_warn_if_fail (priv->dhcp6_ip6_config == NULL);
 	g_clear_object (&priv->dhcp6_ip6_config);
 
 	connection = nm_device_get_connection (self);
 	g_assert (connection);
 	s_ip6 = nm_connection_get_setting_ip6_config (connection);
 	if (!nm_setting_ip_config_get_may_fail (s_ip6) ||
 	    !strcmp (nm_setting_ip_config_get_method (s_ip6), NM_SETTING_IP6_CONFIG_METHOD_DHCP))
 		nm_device_add_pending_action (self, PENDING_ACTION_DHCP6, TRUE);
 
 	if (wait_for_ll) {
 		NMActStageReturn ret;
 
 		 
 		ret = linklocal6_start (self);
 		if (ret == NM_ACT_STAGE_RETURN_POSTPONE) {
 			 
 			return TRUE;
 		}
 
 		 
 		g_assert (ret == NM_ACT_STAGE_RETURN_SUCCESS);
 	}
 
 	if (!dhcp6_start_with_link_ready (self, connection)) {
 		*reason = NM_DEVICE_STATE_REASON_DHCP_START_FAILED;
 		return FALSE;
 	}
 
 	return TRUE;
 }
 
 gboolean
 nm_device_dhcp6_renew (NMDevice *self, gboolean release)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	g_return_val_if_fail (priv->dhcp6_client != NULL, FALSE);
 
 	_LOGI (LOGD_DHCP6, "DHCPv6 lease renewal requested");
 
 	 
 	dhcp6_cleanup (self, TRUE, release);
 
 	 
 	return dhcp6_start (self, FALSE, NULL);
 }
 
  
 
 static gboolean
 have_ip6_address (const NMIP6Config *ip6_config, gboolean linklocal)
 {
 	guint i;
 
 	if (!ip6_config)
 		return FALSE;
 
 	linklocal = !!linklocal;
 
 	for (i = 0; i < nm_ip6_config_get_num_addresses (ip6_config); i++) {
 		const NMPlatformIP6Address *addr = nm_ip6_config_get_address (ip6_config, i);
 
 		if ((IN6_IS_ADDR_LINKLOCAL (&addr->address) == linklocal) &&
 		    !(addr->flags & IFA_F_TENTATIVE))
 			return TRUE;
 	}
 
 	return FALSE;
 }
 
 static void
 linklocal6_cleanup (NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 
 	if (priv->linklocal6_timeout_id) {
 		g_source_remove (priv->linklocal6_timeout_id);
 		priv->linklocal6_timeout_id = 0;
 	}
 }
 
 static gboolean
 linklocal6_timeout_cb (gpointer user_data)
 {
 	NMDevice *self = user_data;
 
 	linklocal6_cleanup (self);
 
 	_LOGD (LOGD_DEVICE, "linklocal6: waiting for link-local addresses failed due to timeout");
 
 	nm_device_activate_schedule_ip6_config_timeout (self);
 	return G_SOURCE_REMOVE;
 }
 
 static void
 linklocal6_complete (NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	NMConnection *connection;
 	const char *method;
 
 	g_assert (priv->linklocal6_timeout_id);
 	g_assert (have_ip6_address (priv->ip6_config, TRUE));
 
 	linklocal6_cleanup (self);
 
 	connection = nm_device_get_connection (self);
 	g_assert (connection);
 
 	method = nm_utils_get_ip_config_method (connection, NM_TYPE_SETTING_IP6_CONFIG);
 
 	_LOGD (LOGD_DEVICE, "linklocal6: waiting for link-local addresses successful, continue with method %s", method);
 
 	if (strcmp (method, NM_SETTING_IP6_CONFIG_METHOD_AUTO) == 0) {
 		if (!addrconf6_start_with_link_ready (self)) {
 			 
 			nm_device_activate_schedule_ip6_config_timeout (self);
 		}
 	} else if (strcmp (method, NM_SETTING_IP6_CONFIG_METHOD_DHCP) == 0) {
 		if (!dhcp6_start_with_link_ready (self, connection)) {
 			 
 			nm_device_activate_schedule_ip6_config_timeout (self);
 		}
 	} else if (strcmp (method, NM_SETTING_IP6_CONFIG_METHOD_LINK_LOCAL) == 0)
 		nm_device_activate_schedule_ip6_config_result (self);
 	else
 		g_return_if_fail (FALSE);
 }
 
 static void
 check_and_add_ipv6ll_addr (NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	int ip_ifindex = nm_device_get_ip_ifindex (self);
 	NMUtilsIPv6IfaceId iid;
 	struct in6_addr lladdr;
 	guint i, n;
 
 	if (priv->nm_ipv6ll == FALSE)
 		return;
 
 	if (priv->ip6_config) {
 		n = nm_ip6_config_get_num_addresses (priv->ip6_config);
 		for (i = 0; i < n; i++) {
 			const NMPlatformIP6Address *addr;
 
 			addr = nm_ip6_config_get_address (priv->ip6_config, i);
 			if (IN6_IS_ADDR_LINKLOCAL (&addr->address)) {
 				 
 				return;
 			}
 		}
 	}
 
 	if (!nm_device_get_ip_iface_identifier (self, &iid)) {
 		_LOGW (LOGD_IP6, "failed to get interface identifier; IPv6 may be broken");
 		return;
 	}
 
 	memset (&lladdr, 0, sizeof (lladdr));
 	lladdr.s6_addr16[0] = htons (0xfe80);
 	nm_utils_ipv6_addr_set_interface_identfier (&lladdr, iid);
 	_LOGD (LOGD_IP6, "adding IPv6LL address %s", nm_utils_inet6_ntop (&lladdr, NULL));
 	if (!nm_platform_ip6_address_add (ip_ifindex,
 	                                  lladdr,
 	                                  in6addr_any,
 	                                  64,
 	                                  NM_PLATFORM_LIFETIME_PERMANENT,
 	                                  NM_PLATFORM_LIFETIME_PERMANENT,
 	                                  0)) {
 		_LOGW (LOGD_IP6, "failed to add IPv6 link-local address %s",
 		       nm_utils_inet6_ntop (&lladdr, NULL));
 	}
 }
 
 static NMActStageReturn
 linklocal6_start (NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
 	NMConnection *connection;
 	const char *method;
 
 	linklocal6_cleanup (self);
 
 	if (have_ip6_address (priv->ip6_config, TRUE))
 		return NM_ACT_STAGE_RETURN_SUCCESS;
 
 	connection = nm_device_get_connection (self);
 	g_assert (connection);
 
 	method = nm_utils_get_ip_config_method (connection, NM_TYPE_SETTING_IP6_CONFIG);
 	_LOGD (LOGD_DEVICE, "linklocal6: starting IPv6 with method '%s', but the device has no link-local addresses configured. Wait.", method);
 
 	check_and_add_ipv6ll_addr (self);
 
 	priv->linklocal6_timeout_id = g_timeout_add_seconds (5, linklocal6_timeout_cb, self);
 
 	return NM_ACT_STAGE_RETURN_POSTPONE;
 }
 
  
 
 static void
 print_support_extended_ifa_flags (NMSettingIP6ConfigPrivacy use_tempaddr)
 {
 	static gint8 warn = 0;
 	static gint8 s_libnl = -1, s_kernel;
 
 	if (warn >= 2)
 		return;
 
 	if (s_libnl == -1) {
 		s_libnl = !!nm_platform_check_support_libnl_extended_ifa_flags ();
 		s_kernel = !!nm_platform_check_support_kernel_extended_ifa_flags ();
 
 		if (s_libnl && s_kernel) {
 			nm_log_dbg (LOGD_IP6, "kernel and libnl support extended IFA_FLAGS (needed by NM for IPv6 private addresses)");
 			warn = 2;
 			return;
 		}
 	}
 
 	if (   use_tempaddr != NM_SETTING_IP6_CONFIG_PRIVACY_PREFER_TEMP_ADDR
 	    && use_tempaddr != NM_SETTING_IP6_CONFIG_PRIVACY_PREFER_PUBLIC_ADDR) {
 		if (warn == 0) {
 			nm_log_dbg (LOGD_IP6, "%s%s%s %s not support extended IFA_FLAGS (needed by NM for IPv6 private addresses)",
 			                      !s_kernel ? "kernel" : "",
 			                      !s_kernel && !s_libnl ? " and " : "",
 			                      !s_libnl ? "libnl" : "",
 			                      !s_kernel && !s_libnl ? "do" : "does");
 			warn = 1;
 		}
 		return;
 	}
 
 	if (!s_libnl && !s_kernel) {
 		nm_log_warn (LOGD_IP6, "libnl and the kernel do not support extended IFA_FLAGS needed by NM for "
 		                       "IPv6 private addresses. This feature is not available");
 	} else if (!s_libnl) {
 		nm_log_warn (LOGD_IP6, "libnl does not support extended IFA_FLAGS needed by NM for "
 		                       "IPv6 private addresses. This feature is not available");
 	} else if (!s_kernel) {
 		nm_log_warn (LOGD_IP6, "The kernel does not support extended IFA_FLAGS needed by NM for "
 		                       "IPv6 private addresses. This feature is not available");
 	}
 
 	warn = 2;
 }
 
 static void
 rdisc_config_changed (NMRDisc *rdisc, NMRDiscConfigMap changed, NMDevice *self)
 {
 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
  	warn = 2;
  }
  
// static void nm_device_ipv6_set_mtu (NMDevice *self, guint32 mtu);
// 
// static void
// nm_device_set_mtu (NMDevice *self, guint32 mtu)
// {
// 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
// 	int ifindex = nm_device_get_ifindex (self);
// 
// 	if (mtu)
// 		priv->mtu = mtu;
// 
// 	 
// 	if (priv->ip6_mtu)
// 		nm_device_ipv6_set_mtu (self, priv->ip6_mtu);
// 
// 	if (priv->mtu != nm_platform_link_get_mtu (ifindex))
// 		nm_platform_link_set_mtu (ifindex, priv->mtu);
// }
// 
// static void
// nm_device_ipv6_set_mtu (NMDevice *self, guint32 mtu)
// {
// 	NMDevicePrivate *priv = NM_DEVICE_GET_PRIVATE (self);
// 	guint32 plat_mtu = nm_device_ipv6_sysctl_get_int32 (self, "mtu", priv->mtu);
// 	char val[16];
// 
// 	priv->ip6_mtu = mtu ?: plat_mtu;
// 
// 	if (priv->ip6_mtu && priv->mtu < priv->ip6_mtu) {
// 		_LOGW (LOGD_DEVICE | LOGD_IP6, "Lowering IPv6 MTU (%d) to match device MTU (%d)",
// 		       priv->ip6_mtu, priv->mtu);
// 		priv->ip6_mtu = priv->mtu;
// 	}
// 
// 	if (priv->ip6_mtu < 1280) {
// 		_LOGW (LOGD_DEVICE | LOGD_IP6, "IPv6 MTU (%d) smaller than 1280, adjusting",
// 		       priv->ip6_mtu);
// 		priv->ip6_mtu = 1280;
// 	}
// 
// 	if (priv->mtu < priv->ip6_mtu) {
// 		_LOGW (LOGD_DEVICE | LOGD_IP6, "Raising device MTU (%d) to match IPv6 MTU (%d)",
// 		       priv->mtu, priv->ip6_mtu);
// 		nm_device_set_mtu (self, priv->ip6_mtu);
// 	}
// 
// 	if (priv->ip6_mtu != plat_mtu) {
// 		g_snprintf (val, sizeof (val), "%d", mtu);
// 		nm_device_ipv6_sysctl_set (self, "mtu", val);
// 	}
// }
// 
  static void
  rdisc_config_changed (NMRDisc *rdisc, NMRDiscConfigMap changed, NMDevice *self)
  {
 			address.preferred = discovered_address->preferred;
 			if (address.preferred > address.lifetime)
 				address.preferred = address.lifetime;
 			address.source = NM_IP_CONFIG_SOURCE_RDISC;
 			address.flags = ifa_flags;
 
 			nm_ip6_config_add_address (priv->ac_ip6_config, &address);
 		}
 	}
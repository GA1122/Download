dbus_send_state_signal(vrrp_t *vrrp)
{
	gchar *object_path;
	GVariant *args;

	 
	if (global_connection == NULL)
		return;

	object_path = dbus_object_create_path_instance(IF_NAME(IF_BASE_IFP(vrrp->ifp)), vrrp->vrid, vrrp->family);

	args = g_variant_new("(u)", vrrp->state);
	dbus_emit_signal(global_connection, object_path, DBUS_VRRP_INSTANCE_INTERFACE, "VrrpStatusChange", args);
	g_free(object_path);
}
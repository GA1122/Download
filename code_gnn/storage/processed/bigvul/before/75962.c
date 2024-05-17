on_bus_acquired(GDBusConnection *connection,
		const gchar     *name,
		__attribute__((unused)) gpointer user_data)
{
	global_connection = connection;
	gchar *path;
	element e;
	GError *local_error = NULL;

	log_message(LOG_INFO, "Acquired DBus bus %s", name);

	 
	path = dbus_object_create_path_vrrp();
	guint vrrp = g_dbus_connection_register_object(connection, path,
							 vrrp_introspection_data->interfaces[0],
							 &interface_vtable, NULL, NULL, &local_error);
	g_hash_table_insert(objects, "__Vrrp__", GUINT_TO_POINTER(vrrp));
	g_free(path);
	if (local_error != NULL) {
		log_message(LOG_INFO, "Registering VRRP object on %s failed: %s",
			    path, local_error->message);
		g_clear_error(&local_error);
	}

	 
	if (LIST_ISEMPTY(vrrp_data->vrrp))
		return;

	for (e = LIST_HEAD(vrrp_data->vrrp); e; ELEMENT_NEXT(e)) {
		vrrp_t * vrrp = ELEMENT_DATA(e);
		dbus_create_object(vrrp);
	}

	 
	path = dbus_object_create_path_vrrp();
	dbus_emit_signal(global_connection, path, DBUS_VRRP_INTERFACE, "VrrpStarted", NULL);
	g_free(path);

	 
	for (e = LIST_HEAD(vrrp_data->vrrp); e; ELEMENT_NEXT(e)) {
		vrrp_t * vrrp = ELEMENT_DATA(e);
		dbus_send_state_signal(vrrp);
	}
}
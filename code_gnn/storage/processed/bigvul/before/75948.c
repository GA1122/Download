dbus_main(__attribute__ ((unused)) void *unused)
{
	gchar *introspection_xml;
	guint owner_id;
	const char *service_name;

	objects = g_hash_table_new(g_str_hash, g_str_equal);

	 
#ifdef DBUS_NEED_G_TYPE_INIT
	g_type_init();
#endif
	GError *error = NULL;

	 
	introspection_xml = read_file(DBUS_VRRP_INTERFACE_FILE_PATH);
	if (!introspection_xml)
		return NULL;
	vrrp_introspection_data = g_dbus_node_info_new_for_xml(introspection_xml, &error);
	FREE(introspection_xml);
	if (error != NULL) {
		log_message(LOG_INFO, "Parsing DBus interface %s from file %s failed: %s",
			    DBUS_VRRP_INTERFACE, DBUS_VRRP_INTERFACE_FILE_PATH, error->message);
		g_clear_error(&error);
		return NULL;
	}

	introspection_xml = read_file(DBUS_VRRP_INSTANCE_INTERFACE_FILE_PATH);
	if (!introspection_xml)
		return NULL;
	vrrp_instance_introspection_data = g_dbus_node_info_new_for_xml(introspection_xml, &error);
	FREE(introspection_xml);
	if (error != NULL) {
		log_message(LOG_INFO, "Parsing DBus interface %s from file %s failed: %s",
			    DBUS_VRRP_INSTANCE_INTERFACE, DBUS_VRRP_INSTANCE_INTERFACE_FILE_PATH, error->message);
		g_clear_error(&error);
		return NULL;
	}

	service_name = global_data->dbus_service_name ? global_data->dbus_service_name : DBUS_SERVICE_NAME;
	owner_id = g_bus_own_name(G_BUS_TYPE_SYSTEM,
				  service_name,
				  G_BUS_NAME_OWNER_FLAGS_NONE,
				  on_bus_acquired,
				  on_name_acquired,
				  on_name_lost,
				  NULL,   
				  NULL);  

	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);

	 
	g_main_loop_unref(loop);
	g_bus_unown_name(owner_id);
	global_connection = NULL;

	sem_post(&thread_end);
	pthread_exit(0);
}

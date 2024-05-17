static bool cgm_dbus_connect(void)
{
	DBusError dbus_error;
	static DBusConnection *connection;

	cgm_lock();
	if (!dbus_threads_initialized) {
		dbus_threads_init_default();
		dbus_threads_initialized = true;
	}

	dbus_error_init(&dbus_error);

	connection = dbus_connection_open_private(CGMANAGER_DBUS_SOCK, &dbus_error);
	if (!connection) {
		DEBUG("Failed opening dbus connection: %s: %s",
				dbus_error.name, dbus_error.message);
		dbus_error_free(&dbus_error);
		cgm_unlock();
		return false;
	}
	dbus_connection_set_exit_on_disconnect(connection, FALSE);
	dbus_error_free(&dbus_error);
	cgroup_manager = nih_dbus_proxy_new(NULL, connection,
				NULL  ,
				"/org/linuxcontainers/cgmanager", NULL, NULL);
	dbus_connection_unref(connection);
	if (!cgroup_manager) {
		NihError *nerr;
		nerr = nih_error_get();
		ERROR("Error opening cgmanager proxy: %s", nerr->message);
		nih_free(nerr);
		cgm_dbus_disconnect();
		return false;
	}

	if (cgmanager_get_api_version_sync(NULL, cgroup_manager, &api_version) != 0) {
		NihError *nerr;
		nerr = nih_error_get();
		ERROR("Error cgroup manager api version: %s", nerr->message);
		nih_free(nerr);
		cgm_dbus_disconnect();
		return false;
	}
	if (api_version < CGM_SUPPORTS_NAMED)
		cull_user_controllers();
	return true;
}
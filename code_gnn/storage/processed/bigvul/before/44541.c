static void do_cgm_set(const char *name, const char *lxcpath, const char *filename, const char *value, int outp)
{
	char *controller, *key, *cgroup = NULL;
	int retval = 0;   
	int ret;
	char *cglast;

	controller = alloca(strlen(filename)+1);
	strcpy(controller, filename);
	key = strchr(controller, '.');
	if (!key) {
		ret = write(outp, &retval, sizeof(retval));
		if (ret != sizeof(retval))
			WARN("Failed to warn cgm_set of error; parent may hang");
		exit(1);
	}
	*key = '\0';

	if (!cgm_dbus_connect()) {
		ERROR("Error connecting to cgroup manager");
		ret = write(outp, &retval, sizeof(retval));
		if (ret != sizeof(retval))
			WARN("Failed to warn cgm_set of error; parent may hang");
		exit(1);
	}
	cgroup = try_get_abs_cgroup(name, lxcpath, controller);
	if (!cgroup) {
		cgm_dbus_disconnect();
		ret = write(outp, &retval, sizeof(retval));
		if (ret != sizeof(retval))
			WARN("Failed to warn cgm_set of error; parent may hang");
		exit(1);
	}
	cglast = strrchr(cgroup, '/');
	if (!cglast) {
		cgm_dbus_disconnect();
		free_abs_cgroup(cgroup);
		ret = write(outp, &retval, sizeof(retval));
		if (ret != sizeof(retval))
			WARN("Failed to warn cgm_set of error; parent may hang");
		exit(1);
	}
	*cglast = '\0';
	if (!lxc_cgmanager_enter(getpid(), controller, cgroup, abs_cgroup_supported())) {
		ERROR("Failed to enter container cgroup %s:%s", controller, cgroup);
		ret = write(outp, &retval, sizeof(retval));
		if (ret != sizeof(retval))
			WARN("Failed to warn cgm_set of error; parent may hang");
		cgm_dbus_disconnect();
		free_abs_cgroup(cgroup);
		exit(1);
	}
	if (cgmanager_set_value_sync(NULL, cgroup_manager, controller, cglast+1, filename, value) != 0) {
		NihError *nerr;
		nerr = nih_error_get();
		ERROR("Error setting cgroup value %s for %s:%s", filename, controller, cgroup);
		ERROR("call to cgmanager_set_value_sync failed: %s", nerr->message);
		nih_free(nerr);
		free_abs_cgroup(cgroup);
		cgm_dbus_disconnect();
		ret = write(outp, &retval, sizeof(retval));
		if (ret != sizeof(retval))
			WARN("Failed to warn cgm_set of error; parent may hang");
		exit(1);
	}
	free_abs_cgroup(cgroup);
	cgm_dbus_disconnect();
	 
	retval = 1;
	ret = write(outp, &retval, sizeof(retval));
	if (ret != sizeof(retval)) {
		exit(1);
	}
	exit(0);
}
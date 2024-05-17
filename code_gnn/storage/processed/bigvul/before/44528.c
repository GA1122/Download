struct cgroup_ops *cgm_ops_init(void)
{
	if (!collect_subsytems())
		return NULL;
	if (!cgm_dbus_connect())
		goto err1;

	if (geteuid() == 0 && !lxc_cgmanager_escape())
		goto err2;
	cgm_dbus_disconnect();

	return &cgmanager_ops;

err2:
	cgm_dbus_disconnect();
err1:
	free_subsystems();
	return NULL;
}
static bool cgm_setup_limits(void *hdata, struct lxc_list *cgroup_settings, bool do_devices)
{
	struct cgm_data *d = hdata;
	struct lxc_list *iterator, *sorted_cgroup_settings, *next;
	struct lxc_cgroup *cg;
	bool ret = false;

	if (lxc_list_empty(cgroup_settings))
		return true;

	if (!d || !d->cgroup_path)
		return false;

	if (!cgm_dbus_connect()) {
		ERROR("Error connecting to cgroup manager");
		return false;
	}

	sorted_cgroup_settings = sort_cgroup_settings(cgroup_settings);
	if (!sorted_cgroup_settings) {
		return false;
	}

	lxc_list_for_each(iterator, sorted_cgroup_settings) {
		char controller[100], *p;
		cg = iterator->elem;
		if (do_devices != !strncmp("devices", cg->subsystem, 7))
			continue;
		if (strlen(cg->subsystem) > 100)  
			goto out;
		strcpy(controller, cg->subsystem);
		p = strchr(controller, '.');
		if (p)
			*p = '\0';
		if (cgmanager_set_value_sync(NULL, cgroup_manager, controller,
					 d->cgroup_path, cg->subsystem, cg->value) != 0) {
			NihError *nerr;
			nerr = nih_error_get();
			ERROR("call to cgmanager_set_value_sync failed: %s", nerr->message);
			nih_free(nerr);
			ERROR("Error setting cgroup %s:%s limit type %s", controller,
				d->cgroup_path, cg->subsystem);
			goto out;
		}

		DEBUG("cgroup '%s' set to '%s'", cg->subsystem, cg->value);
	}

	ret = true;
	INFO("cgroup limits have been setup");
out:
	lxc_list_for_each_safe(iterator, sorted_cgroup_settings, next) {
		lxc_list_del(iterator);
		free(iterator);
	}
	free(sorted_cgroup_settings);
	cgm_dbus_disconnect();
	return ret;
}
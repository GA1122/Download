static bool fetch_seccomp(struct lxc_container *c,
			  lxc_attach_options_t *options)
{
	char *path;

	if (!(options->namespaces & CLONE_NEWNS) || !(options->attach_flags & LXC_ATTACH_LSM)) {
		free(c->lxc_conf->seccomp);
		c->lxc_conf->seccomp = NULL;
		return true;
	}

	 
	if (!c->set_config_item(c, "lxc.seccomp", "")) {
		return false;
	}

	 
	path = c->get_running_config_item(c, "lxc.seccomp");
	if (!path) {
		INFO("Failed to get running config item for lxc.seccomp.");
		return true;
	}

	 
	if (!c->set_config_item(c, "lxc.seccomp", path)) {
		free(path);
		return false;
	}
	free(path);

	 
	if (lxc_read_seccomp_config(c->lxc_conf) < 0) {
		ERROR("Error reading seccomp policy");
		return false;
	}

	INFO("Retrieved seccomp policy.");
	return true;
}
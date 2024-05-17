static bool no_new_privs(struct lxc_container *c,
			 lxc_attach_options_t *options)
{
	char *val;

	 
	if (!c->set_config_item(c, "lxc.no_new_privs", "")) {
		return false;
	}

	 
	val = c->get_running_config_item(c, "lxc.no_new_privs");
	if (!val) {
		INFO("Failed to get running config item for lxc.no_new_privs.");
		return false;
	}

	 
	if (!c->set_config_item(c, "lxc.no_new_privs", val)) {
		free(val);
		return false;
	}
	free(val);

	return true;
}

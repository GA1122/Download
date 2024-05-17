static int __init apparmor_init(void)
{
	int error;

	if (!apparmor_enabled || !security_module_enable("apparmor")) {
		aa_info_message("AppArmor disabled by boot time parameter");
		apparmor_enabled = 0;
		return 0;
	}

	error = aa_alloc_root_ns();
	if (error) {
		AA_ERROR("Unable to allocate default profile namespace\n");
		goto alloc_out;
	}

	error = set_init_cxt();
	if (error) {
		AA_ERROR("Failed to set context on init task\n");
		aa_free_root_ns();
		goto alloc_out;
	}
	security_add_hooks(apparmor_hooks, ARRAY_SIZE(apparmor_hooks));

	 
	apparmor_initialized = 1;
	if (aa_g_profile_mode == APPARMOR_COMPLAIN)
		aa_info_message("AppArmor initialized: complain mode enabled");
	else if (aa_g_profile_mode == APPARMOR_KILL)
		aa_info_message("AppArmor initialized: kill mode enabled");
	else
		aa_info_message("AppArmor initialized");

	return error;

alloc_out:
	aa_destroy_aafs();

	apparmor_enabled = 0;
	return error;
}
static void cdrom_sysctl_register(void)
{
	static int initialized;

	if (initialized == 1)
		return;

	cdrom_sysctl_header = register_sysctl_table(cdrom_root_table);

	 
	cdrom_sysctl_settings.autoclose = autoclose;
	cdrom_sysctl_settings.autoeject = autoeject;
	cdrom_sysctl_settings.debug = debug;
	cdrom_sysctl_settings.lock = lockdoor;
	cdrom_sysctl_settings.check = check_media_type;

	initialized = 1;
}

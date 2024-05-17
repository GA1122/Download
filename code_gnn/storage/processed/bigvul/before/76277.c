static int cdrom_sysctl_handler(struct ctl_table *ctl, int write,
				void __user *buffer, size_t *lenp, loff_t *ppos)
{
	int ret;
	
	ret = proc_dointvec(ctl, write, buffer, lenp, ppos);

	if (write) {
	
		 
		autoclose        = !!cdrom_sysctl_settings.autoclose;
		autoeject        = !!cdrom_sysctl_settings.autoeject;
		debug	         = !!cdrom_sysctl_settings.debug;
		lockdoor         = !!cdrom_sysctl_settings.lock;
		check_media_type = !!cdrom_sysctl_settings.check;

		 
		cdrom_update_settings();
	}

        return ret;
}

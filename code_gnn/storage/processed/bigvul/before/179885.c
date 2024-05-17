 static int lxc_mount_auto_mounts(struct lxc_conf *conf, int flags, struct lxc_handler *handler)
 {
 	int r;
 	size_t i;
 	static struct {
 		int match_mask;
 		int match_flag;
 		const char *source;
 		const char *destination;
 		const char *fstype;
 		unsigned long flags;
 		const char *options;
 	} default_mounts[] = {
 		 
  		{ LXC_AUTO_PROC_MASK, LXC_AUTO_PROC_MIXED, "proc",                                              "%r/proc",                      "proc",     MS_NODEV|MS_NOEXEC|MS_NOSUID,   NULL },
		{ LXC_AUTO_PROC_MASK, LXC_AUTO_PROC_MIXED, "%r/proc/sys/net",                                   "%r/proc/net",                  NULL,       MS_BIND,                        NULL },
// 		 
// 		{ LXC_AUTO_PROC_MASK, LXC_AUTO_PROC_MIXED, "%r/proc/sys/net",                                   "%r/proc/tty",                  NULL,       MS_BIND,                        NULL },
  		{ LXC_AUTO_PROC_MASK, LXC_AUTO_PROC_MIXED, "%r/proc/sys",                                       "%r/proc/sys",                  NULL,       MS_BIND,                        NULL },
  		{ LXC_AUTO_PROC_MASK, LXC_AUTO_PROC_MIXED, NULL,                                                "%r/proc/sys",                  NULL,       MS_REMOUNT|MS_BIND|MS_RDONLY,   NULL },
		{ LXC_AUTO_PROC_MASK, LXC_AUTO_PROC_MIXED, "%r/proc/net",                                       "%r/proc/sys/net",              NULL,       MS_MOVE,                        NULL },
// 		{ LXC_AUTO_PROC_MASK, LXC_AUTO_PROC_MIXED, "%r/proc/tty",                                       "%r/proc/sys/net",              NULL,       MS_MOVE,                        NULL },
  		{ LXC_AUTO_PROC_MASK, LXC_AUTO_PROC_MIXED, "%r/proc/sysrq-trigger",                             "%r/proc/sysrq-trigger",        NULL,       MS_BIND,                        NULL },
  		{ LXC_AUTO_PROC_MASK, LXC_AUTO_PROC_MIXED, NULL,                                                "%r/proc/sysrq-trigger",        NULL,       MS_REMOUNT|MS_BIND|MS_RDONLY,   NULL },
  		{ LXC_AUTO_PROC_MASK, LXC_AUTO_PROC_RW,    "proc",                                              "%r/proc",                      "proc",     MS_NODEV|MS_NOEXEC|MS_NOSUID,   NULL },
 		{ LXC_AUTO_SYS_MASK,  LXC_AUTO_SYS_RW,     "sysfs",                                             "%r/sys",                       "sysfs",    0,                              NULL },
 		{ LXC_AUTO_SYS_MASK,  LXC_AUTO_SYS_RO,     "sysfs",                                             "%r/sys",                       "sysfs",    MS_RDONLY,                      NULL },
 		{ LXC_AUTO_SYS_MASK,  LXC_AUTO_SYS_MIXED,  "sysfs",                                             "%r/sys",                       "sysfs",    MS_NODEV|MS_NOEXEC|MS_NOSUID,   NULL },
 		{ LXC_AUTO_SYS_MASK,  LXC_AUTO_SYS_MIXED,  "%r/sys",                                            "%r/sys",                       NULL,       MS_BIND,                        NULL },
 		{ LXC_AUTO_SYS_MASK,  LXC_AUTO_SYS_MIXED,  NULL,                                                "%r/sys",                       NULL,       MS_REMOUNT|MS_BIND|MS_RDONLY,   NULL },
 		{ LXC_AUTO_SYS_MASK,  LXC_AUTO_SYS_MIXED,  "sysfs",                                             "%r/sys/devices/virtual/net",   "sysfs",    0,                              NULL },
 		{ LXC_AUTO_SYS_MASK,  LXC_AUTO_SYS_MIXED,  "%r/sys/devices/virtual/net/devices/virtual/net",    "%r/sys/devices/virtual/net",   NULL,       MS_BIND,                        NULL },
 		{ LXC_AUTO_SYS_MASK,  LXC_AUTO_SYS_MIXED,  NULL,                                                "%r/sys/devices/virtual/net",   NULL,       MS_REMOUNT|MS_BIND|MS_NOSUID|MS_NODEV|MS_NOEXEC,   NULL },
 		{ 0,                  0,                   NULL,                                                NULL,                           NULL,       0,                              NULL }
 	};
 
 	for (i = 0; default_mounts[i].match_mask; i++) {
 		if ((flags & default_mounts[i].match_mask) == default_mounts[i].match_flag) {
 			char *source = NULL;
 			char *destination = NULL;
 			int saved_errno;
 			unsigned long mflags;
 
 			if (default_mounts[i].source) {
 				 
 				source = lxc_string_replace("%r", conf->rootfs.path ? conf->rootfs.mount : "", default_mounts[i].source);
 				if (!source) {
 					SYSERROR("memory allocation error");
 					return -1;
 				}
 			}
 			if (default_mounts[i].destination) {
 				 
 				destination = lxc_string_replace("%r", conf->rootfs.path ? conf->rootfs.mount : "", default_mounts[i].destination);
 				if (!destination) {
 					saved_errno = errno;
 					SYSERROR("memory allocation error");
 					free(source);
 					errno = saved_errno;
 					return -1;
 				}
  			}
  			mflags = add_required_remount_flags(source, destination,
  					default_mounts[i].flags);
			r = mount(source, destination, default_mounts[i].fstype, mflags, default_mounts[i].options);
// 			r = safe_mount(source, destination, default_mounts[i].fstype, mflags, default_mounts[i].options, conf->rootfs.path ? conf->rootfs.mount : NULL);
  			saved_errno = errno;
  			if (r < 0 && errno == ENOENT) {
  				INFO("Mount source or target for %s on %s doesn't exist. Skipping.", source, destination);
 				r = 0;
 			}
 			else if (r < 0)
 				SYSERROR("error mounting %s on %s flags %lu", source, destination, mflags);
 
 			free(source);
 			free(destination);
 			if (r < 0) {
 				errno = saved_errno;
 				return -1;
 			}
 		}
 	}
 
 	if (flags & LXC_AUTO_CGROUP_MASK) {
 		int cg_flags;
 
 		cg_flags = flags & LXC_AUTO_CGROUP_MASK;
 		 
 		if (cg_flags == LXC_AUTO_CGROUP_NOSPEC || cg_flags == LXC_AUTO_CGROUP_FULL_NOSPEC) {
 			int has_sys_admin = 0;
 			if (!lxc_list_empty(&conf->keepcaps)) {
 				has_sys_admin = in_caplist(CAP_SYS_ADMIN, &conf->keepcaps);
 			} else {
 				has_sys_admin = !in_caplist(CAP_SYS_ADMIN, &conf->caps);
 			}
 			if (cg_flags == LXC_AUTO_CGROUP_NOSPEC) {
 				cg_flags = has_sys_admin ? LXC_AUTO_CGROUP_RW : LXC_AUTO_CGROUP_MIXED;
 			} else {
 				cg_flags = has_sys_admin ? LXC_AUTO_CGROUP_FULL_RW : LXC_AUTO_CGROUP_FULL_MIXED;
 			}
 		}
 
 		if (!cgroup_mount(conf->rootfs.path ? conf->rootfs.mount : "", handler, cg_flags)) {
 			SYSERROR("error mounting /sys/fs/cgroup");
 			return -1;
 		}
 	}
 
 	return 0;
 }
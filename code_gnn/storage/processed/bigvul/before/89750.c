static void mount_directories(void) {
	if (arg_debug || arg_debug_private_lib)
		printf("Mount-bind %s on top of /lib /lib64 /usr/lib\n", RUN_LIB_DIR);

	if (is_dir("/lib")) {
		if (mount(RUN_LIB_DIR, "/lib", NULL, MS_BIND|MS_REC, NULL) < 0 ||
			mount(NULL, "/lib", NULL, MS_BIND|MS_REMOUNT|MS_NOSUID|MS_NODEV|MS_REC, NULL) < 0)
			errExit("mount bind");
		fs_logger2("tmpfs", "/lib");
		fs_logger("mount /lib");
	}

	if (is_dir("/lib64")) {
		if (mount(RUN_LIB_DIR, "/lib64", NULL, MS_BIND|MS_REC, NULL) < 0 ||
			mount(NULL, "/lib64", NULL, MS_BIND|MS_REMOUNT|MS_NOSUID|MS_NODEV|MS_REC, NULL) < 0)
			errExit("mount bind");
		fs_logger2("tmpfs", "/lib64");
		fs_logger("mount /lib64");
	}

	if (is_dir("/usr/lib")) {
		if (mount(RUN_LIB_DIR, "/usr/lib", NULL, MS_BIND|MS_REC, NULL) < 0 ||
			mount(NULL, "/usr/lib", NULL, MS_BIND|MS_REMOUNT|MS_NOSUID|MS_NODEV|MS_REC, NULL) < 0)
			errExit("mount bind");
		fs_logger2("tmpfs", "/usr/lib");
		fs_logger("mount /usr/lib");
	}

	if (is_dir("/lib32")) {
		if (mount(RUN_RO_DIR, "/lib32", "none", MS_BIND, "mode=400,gid=0") < 0)
			errExit("disable file");
		fs_logger("blacklist-nolog /lib32");
	}
	if (is_dir("/libx32")) {
		if (mount(RUN_RO_DIR, "/libx32", "none", MS_BIND, "mode=400,gid=0") < 0)
			errExit("disable file");
		fs_logger("blacklist-nolog /libx32");
	}
}
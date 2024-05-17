void fs_private_lib(void) {
#ifndef __x86_64__
	fwarning("private-lib feature is currently available only on amd64 platforms\n");
	return;
#endif
	char *private_list = cfg.lib_private_keep;
	if (arg_debug || arg_debug_private_lib)
		printf("Starting private-lib processing: program %s, shell %s\n",
			(cfg.original_program_index > 0)? cfg.original_argv[cfg.original_program_index]: "none",
		(arg_shell_none)? "none": cfg.shell);

	mkdir_attr(RUN_LIB_DIR, 0755, 0, 0);

	if (arg_debug || arg_debug_private_lib)
		printf("Installing standard C library\n");
	fslib_install_stdc();

	timetrace_start();

	if (cfg.original_program_index > 0) {
		if (arg_debug || arg_debug_private_lib)
			printf("Installing sandboxed program libraries\n");
		fslib_install_list(cfg.original_argv[cfg.original_program_index]);
	}

	if (!arg_shell_none) {
		if (arg_debug || arg_debug_private_lib)
			printf("Installing shell libraries\n");

		fslib_install_list(cfg.shell);
		fslib_install_list("/bin/ls,/bin/cat,/bin/mv,/bin/rm");

	}

	if (private_list && *private_list != '\0') {
		if (arg_debug || arg_debug_private_lib)
			printf("Processing private-lib files\n");
		fslib_install_list(private_list);
	}

	if (arg_private_bin && cfg.bin_private_lib && *cfg.bin_private_lib != '\0') {
		if (arg_debug || arg_debug_private_lib)
			printf("Processing private-bin files\n");
		fslib_install_list(cfg.bin_private_lib);
	}
	fmessage("Program libraries installed in %0.2f ms\n", timetrace_end());

	if (arg_debug || arg_debug_private_lib)
		printf("Installing system libraries\n");
	fslib_install_system();

	fslib_install_list("/usr/bin/firejail,firejail");  

	fmessage("Installed %d %s and %d %s\n", lib_cnt, (lib_cnt == 1)? "library": "libraries",
		dir_cnt, (dir_cnt == 1)? "directory": "directories");

	mount_directories();
}

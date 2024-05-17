static int label_boot(cmd_tbl_t *cmdtp, struct pxe_label *label)
{
	char *bootm_argv[] = { "bootm", NULL, NULL, NULL, NULL };
	char initrd_str[28];
	char mac_str[29] = "";
	char ip_str[68] = "";
	char *fit_addr = NULL;
	int bootm_argc = 2;
	int len = 0;
	ulong kernel_addr;
	void *buf;

	label_print(label);

	label->attempted = 1;

	if (label->localboot) {
		if (label->localboot_val >= 0)
			label_localboot(label);
		return 0;
	}

	if (!label->kernel) {
		printf("No kernel given, skipping %s\n",
		       label->name);
		return 1;
	}

	if (label->initrd) {
		if (get_relfile_envaddr(cmdtp, label->initrd, "ramdisk_addr_r") < 0) {
			printf("Skipping %s for failure retrieving initrd\n",
			       label->name);
			return 1;
		}

		bootm_argv[2] = initrd_str;
		strncpy(bootm_argv[2], env_get("ramdisk_addr_r"), 18);
		strcat(bootm_argv[2], ":");
		strncat(bootm_argv[2], env_get("filesize"), 9);
		bootm_argc = 3;
	}

	if (get_relfile_envaddr(cmdtp, label->kernel, "kernel_addr_r") < 0) {
		printf("Skipping %s for failure retrieving kernel\n",
		       label->name);
		return 1;
	}

	if (label->ipappend & 0x1) {
		sprintf(ip_str, " ip=%s:%s:%s:%s",
			env_get("ipaddr"), env_get("serverip"),
			env_get("gatewayip"), env_get("netmask"));
	}

#ifdef CONFIG_CMD_NET
	if (label->ipappend & 0x2) {
		int err;

		strcpy(mac_str, " BOOTIF=");
		err = format_mac_pxe(mac_str + 8, sizeof(mac_str) - 8);
		if (err < 0)
			mac_str[0] = '\0';
	}
#endif

	if ((label->ipappend & 0x3) || label->append) {
		char bootargs[CONFIG_SYS_CBSIZE] = "";
		char finalbootargs[CONFIG_SYS_CBSIZE];

		if (strlen(label->append ?: "") +
		    strlen(ip_str) + strlen(mac_str) + 1 > sizeof(bootargs)) {
			printf("bootarg overflow %zd+%zd+%zd+1 > %zd\n",
			       strlen(label->append ?: ""),
			       strlen(ip_str), strlen(mac_str),
			       sizeof(bootargs));
			return 1;
		}

		if (label->append)
			strncpy(bootargs, label->append, sizeof(bootargs));

		strcat(bootargs, ip_str);
		strcat(bootargs, mac_str);

		cli_simple_process_macros(bootargs, finalbootargs);
		env_set("bootargs", finalbootargs);
		printf("append: %s\n", finalbootargs);
	}

	bootm_argv[1] = env_get("kernel_addr_r");
	 
	if (label->config) {
		int len = strlen(bootm_argv[1]) + strlen(label->config) + 1;

		fit_addr = malloc(len);
		if (!fit_addr) {
			printf("malloc fail (FIT address)\n");
			return 1;
		}
		snprintf(fit_addr, len, "%s%s", bootm_argv[1], label->config);
		bootm_argv[1] = fit_addr;
	}

	 
	bootm_argv[3] = env_get("fdt_addr_r");

	 
	if (bootm_argv[3]) {
		char *fdtfile = NULL;
		char *fdtfilefree = NULL;

		if (label->fdt) {
			fdtfile = label->fdt;
		} else if (label->fdtdir) {
			char *f1, *f2, *f3, *f4, *slash;

			f1 = env_get("fdtfile");
			if (f1) {
				f2 = "";
				f3 = "";
				f4 = "";
			} else {
				 
				f1 = env_get("soc");
				f2 = "-";
				f3 = env_get("board");
				f4 = ".dtb";
			}

			len = strlen(label->fdtdir);
			if (!len)
				slash = "./";
			else if (label->fdtdir[len - 1] != '/')
				slash = "/";
			else
				slash = "";

			len = strlen(label->fdtdir) + strlen(slash) +
				strlen(f1) + strlen(f2) + strlen(f3) +
				strlen(f4) + 1;
			fdtfilefree = malloc(len);
			if (!fdtfilefree) {
				printf("malloc fail (FDT filename)\n");
				goto cleanup;
			}

			snprintf(fdtfilefree, len, "%s%s%s%s%s%s",
				 label->fdtdir, slash, f1, f2, f3, f4);
			fdtfile = fdtfilefree;
		}

		if (fdtfile) {
			int err = get_relfile_envaddr(cmdtp, fdtfile,
						      "fdt_addr_r");

			free(fdtfilefree);
			if (err < 0) {
				printf("Skipping %s for failure retrieving fdt\n",
				       label->name);
				goto cleanup;
			}
		} else {
			bootm_argv[3] = NULL;
		}
	}

	if (!bootm_argv[3])
		bootm_argv[3] = env_get("fdt_addr");

	if (bootm_argv[3]) {
		if (!bootm_argv[2])
			bootm_argv[2] = "-";
		bootm_argc = 4;
	}

	kernel_addr = genimg_get_kernel_addr(bootm_argv[1]);
	buf = map_sysmem(kernel_addr, 0);
	 
	if (genimg_get_format(buf) != IMAGE_FORMAT_INVALID)
		do_bootm(cmdtp, 0, bootm_argc, bootm_argv);
#ifdef CONFIG_CMD_BOOTI
	 
	else
		do_booti(cmdtp, 0, bootm_argc, bootm_argv);
#elif defined(CONFIG_CMD_BOOTZ)
	 
	else
		do_bootz(cmdtp, 0, bootm_argc, bootm_argv);
#endif
	unmap_sysmem(buf);

cleanup:
	if (fit_addr)
		free(fit_addr);
	return 1;
}

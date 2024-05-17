int get_pxelinux_path(cmd_tbl_t *cmdtp, const char *file,
		      unsigned long pxefile_addr_r)
{
	size_t base_len = strlen(PXELINUX_DIR);
	char path[MAX_TFTP_PATH_LEN + 1];

	if (base_len + strlen(file) > MAX_TFTP_PATH_LEN) {
		printf("path (%s%s) too long, skipping\n",
		       PXELINUX_DIR, file);
		return -ENAMETOOLONG;
	}

	sprintf(path, PXELINUX_DIR "%s", file);

	return get_pxe_file(cmdtp, path, pxefile_addr_r);
}

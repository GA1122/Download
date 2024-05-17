static int get_relfile(cmd_tbl_t *cmdtp, const char *file_path,
		       unsigned long file_addr)
{
	size_t path_len;
	char relfile[MAX_TFTP_PATH_LEN + 1];
	char addr_buf[18];
	int err;

	err = get_bootfile_path(file_path, relfile, sizeof(relfile));

	if (err < 0)
		return err;

	path_len = strlen(file_path);
	path_len += strlen(relfile);

	if (path_len > MAX_TFTP_PATH_LEN) {
		printf("Base path too long (%s%s)\n", relfile, file_path);

		return -ENAMETOOLONG;
	}

	strcat(relfile, file_path);

	printf("Retrieving file: %s\n", relfile);

	sprintf(addr_buf, "%lx", file_addr);

	return do_getfile(cmdtp, relfile, addr_buf);
}

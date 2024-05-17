int get_pxe_file(cmd_tbl_t *cmdtp, const char *file_path,
		 unsigned long file_addr)
{
	unsigned long config_file_size;
	char *tftp_filesize;
	int err;
	char *buf;

	err = get_relfile(cmdtp, file_path, file_addr);

	if (err < 0)
		return err;

	 
	tftp_filesize = from_env("filesize");

	if (!tftp_filesize)
		return -ENOENT;

	if (strict_strtoul(tftp_filesize, 16, &config_file_size) < 0)
		return -EINVAL;

	buf = map_sysmem(file_addr + config_file_size, 1);
	*buf = '\0';
	unmap_sysmem(buf);

	return 1;
}

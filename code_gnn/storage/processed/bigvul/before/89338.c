static int get_relfile_envaddr(cmd_tbl_t *cmdtp, const char *file_path,
			       const char *envaddr_name)
{
	unsigned long file_addr;
	char *envaddr;

	envaddr = from_env(envaddr_name);

	if (!envaddr)
		return -ENOENT;

	if (strict_strtoul(envaddr, 16, &file_addr) < 0)
		return -EINVAL;

	return get_relfile(cmdtp, file_path, file_addr);
}

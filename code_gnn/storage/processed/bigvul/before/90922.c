bool IsWSL(void)
{
	bool is_wsl = false;

	char buf[1024];

	int status_fd = open("/proc/sys/kernel/osrelease", O_RDONLY);
	if (status_fd == -1)
		return is_wsl;

	ssize_t num_read = read(status_fd, buf, sizeof(buf) - 1);

	if (num_read > 0)
	{
		buf[num_read] = 0;
		is_wsl |= (strstr(buf, "Microsoft") != NULL);
		is_wsl |= (strstr(buf, "WSL") != NULL);
	}

	status_fd = open("/proc/version", O_RDONLY);
	if (status_fd == -1)
		return is_wsl;

	num_read = read(status_fd, buf, sizeof(buf) - 1);

	if (num_read > 0)
	{
		buf[num_read] = 0;
		is_wsl |= (strstr(buf, "Microsoft") != NULL);
		is_wsl |= (strstr(buf, "WSL") != NULL);
	}

	return is_wsl;
}
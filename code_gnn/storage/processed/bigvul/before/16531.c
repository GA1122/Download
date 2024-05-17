write_config_file(const char* pathname) {
	int config_fd = creat(pathname, O_WRONLY);
	if(config_fd == -1) {
		dprintf(D_ALWAYS, "Failed to create configuration file.\n");
		return -1;
	}
	iterate_params(&write_config_variable, &config_fd);
	if(close(config_fd) == -1) {
		dprintf(D_ALWAYS, "Error closing new configuration file.\n");
		return -1;
	}
	return 0;
}

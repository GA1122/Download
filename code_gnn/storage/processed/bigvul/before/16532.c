write_config_variable(const param_info_t* value, void* file_desc) {
	int config_fd = *((int*) file_desc);
	char* actual_value = param(value->name);
	if(strcmp(actual_value, value->str_val) != 0) {
		char output[512];
		snprintf(output, 512, "# %s:  Default value = (%s)\n", value->name, value->str_val);
		if(write(config_fd, &output, 512*sizeof(char)) == -1) {
			dprintf(D_ALWAYS, "Failed to write to configuration file.\n");
			return -1;
		}
		snprintf(output, 512, "%s = %s", value->name, actual_value);
		if(write(config_fd, &output, 512*sizeof(char)) == -1) {
			dprintf(D_ALWAYS, "Failed to write to configuration file.\n");
			return -1;
		}
	}
	return 0;
}

void extract_command_name(int index, char **argv) {
	EUID_ASSERT();
	assert(argv);
	assert(argv[index]);

	cfg.original_program_index = index;

	char *str = strdup(argv[index]);
	if (!str)
		errExit("strdup");


	cfg.command_name = str;
	if (!cfg.command_name)
		errExit("strdup");

	char *ptr = cfg.command_name;
	while (*ptr != ' ' && *ptr != '\t' && *ptr != '\0')
		ptr++;
	*ptr = '\0';

	ptr = strrchr(cfg.command_name, '/');
	if (ptr) {
		ptr++;
		if (*ptr == '\0') {
			fprintf(stderr, "Error: invalid command name\n");
			exit(1);
		}

		char *tmp = strdup(ptr);
		if (!tmp)
			errExit("strdup");

		char *ptr2 = tmp;
		while (*ptr2 != '.' && *ptr2 != '\0')
			ptr2++;
		*ptr2 = '\0';

		free(cfg.command_name);
		cfg.command_name = tmp;
	}
}

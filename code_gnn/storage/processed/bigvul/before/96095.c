void update_map(char *mapping, char *map_file) {
	int fd;
	size_t j;
	size_t map_len;				   

	 

	map_len = strlen(mapping);
	for (j = 0; j < map_len; j++)
		if (mapping[j] == ',')
			mapping[j] = '\n';

	fd = open(map_file, O_RDWR);
	if (fd == -1) {
		fprintf(stderr, "Error: cannot open %s: %s\n", map_file, strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (write(fd, mapping, map_len) != (ssize_t)map_len) {
		fprintf(stderr, "Error: cannot write to %s: %s\n", map_file, strerror(errno));
		exit(EXIT_FAILURE);
	}

	close(fd);
}
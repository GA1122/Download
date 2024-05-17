struct pathname *process_extract_files(struct pathname *path, char *filename)
{
	FILE *fd;
	char buffer[MAX_LINE + 1];  
	char *name;

	fd = fopen(filename, "r");
	if(fd == NULL)
		EXIT_UNSQUASH("Failed to open extract file \"%s\" because %s\n",
			filename, strerror(errno));

	while(fgets(name = buffer, MAX_LINE + 1, fd) != NULL) {
		int len = strlen(name);

		if(len == MAX_LINE && name[len - 1] != '\n')
			 
			EXIT_UNSQUASH("Line too long when reading "
				"extract file \"%s\", larger than %d "
				"bytes\n", filename, MAX_LINE);

		 
		if(len && name[len - 1] == '\n')
			name[len - 1] = '\0';

		 
		while(isspace(*name))
			name ++;

		 
		if(*name == '#')
			continue;

		 
		if(*name == '\\')
			name ++;

		 
		if(*name == '\0')
			continue;

		path = add_path(path, name, name);
	}

	if(ferror(fd))
		EXIT_UNSQUASH("Reading extract file \"%s\" failed because %s\n",
			filename, strerror(errno));

	fclose(fd);
	return path;
}

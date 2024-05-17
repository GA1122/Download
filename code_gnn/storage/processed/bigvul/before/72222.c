load_server_config(const char *filename, Buffer *conf)
{
	char line[4096], *cp;
	FILE *f;
	int lineno = 0;

	debug2("%s: filename %s", __func__, filename);
	if ((f = fopen(filename, "r")) == NULL) {
		perror(filename);
		exit(1);
	}
	buffer_clear(conf);
	while (fgets(line, sizeof(line), f)) {
		lineno++;
		if (strlen(line) == sizeof(line) - 1)
			fatal("%s line %d too long", filename, lineno);
		 
		if ((cp = strchr(line, '#')) != NULL)
			memcpy(cp, "\n", 2);
		cp = line + strspn(line, " \t\r");

		buffer_append(conf, cp, strlen(cp));
	}
	buffer_append(conf, "\0", 1);
	fclose(f);
	debug2("%s: done config len = %d", __func__, buffer_len(conf));
}

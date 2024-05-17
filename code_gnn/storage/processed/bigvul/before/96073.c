char *expand_home(const char *path, const char* homedir) {
	assert(path);
	assert(homedir);

	char *new_name = NULL;
	if (strncmp(path, "${HOME}", 7) == 0) {
		if (asprintf(&new_name, "%s%s", homedir, path + 7) == -1)
			errExit("asprintf");
		return new_name;
	}
	else if (*path == '~') {
		if (asprintf(&new_name, "%s%s", homedir, path + 1) == -1)
			errExit("asprintf");
		return new_name;
	}

	return strdup(path);
}

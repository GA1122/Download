struct pathname *add_path(struct pathname *paths, char *target, char *alltarget)
{
	char *targname;
	int i, error;

	TRACE("add_path: adding \"%s\" extract file\n", target);

	target = get_component(target, &targname);

	if(paths == NULL) {
		paths = malloc(sizeof(struct pathname));
		if(paths == NULL)
			EXIT_UNSQUASH("failed to allocate paths\n");

		paths->names = 0;
		paths->name = NULL;
	}

	for(i = 0; i < paths->names; i++)
		if(strcmp(paths->name[i].name, targname) == 0)
			break;

	if(i == paths->names) {
		 
		paths->names ++;
		paths->name = realloc(paths->name, (i + 1) *
			sizeof(struct path_entry));
		if(paths->name == NULL)
			EXIT_UNSQUASH("Out of memory in add_path\n");	
		paths->name[i].name = targname;
		paths->name[i].paths = NULL;
		if(use_regex) {
			paths->name[i].preg = malloc(sizeof(regex_t));
			if(paths->name[i].preg == NULL)
				EXIT_UNSQUASH("Out of memory in add_path\n");
			error = regcomp(paths->name[i].preg, targname,
				REG_EXTENDED|REG_NOSUB);
			if(error) {
				char str[1024];  

				regerror(error, paths->name[i].preg, str, 1024);
				EXIT_UNSQUASH("invalid regex %s in export %s, "
					"because %s\n", targname, alltarget,
					str);
			}
		} else
			paths->name[i].preg = NULL;

		if(target[0] == '\0')
			 
			paths->name[i].paths = NULL;
		else
			 
			paths->name[i].paths = add_path(NULL, target, alltarget);
	} else {
		 
		free(targname);

		if(paths->name[i].paths == NULL) {
			 
		} else if(target[0] == '\0') {
			 
			free_path(paths->name[i].paths);
			paths->name[i].paths = NULL;
		} else
			 
			add_path(paths->name[i].paths, target, alltarget);
	}

	return paths;
}

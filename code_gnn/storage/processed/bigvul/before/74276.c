int matches(struct pathnames *paths, char *name, struct pathnames **new)
{
	int i, n;

	if(paths == NULL) {
		*new = NULL;
		return TRUE;
	}

	*new = init_subdir();

	for(n = 0; n < paths->count; n++) {
		struct pathname *path = paths->path[n];
		for(i = 0; i < path->names; i++) {
			int match = use_regex ?
				regexec(path->name[i].preg, name, (size_t) 0,
				NULL, 0) == 0 : fnmatch(path->name[i].name,
				name, FNM_PATHNAME|FNM_PERIOD|FNM_EXTMATCH) ==
				0;
			if(match && path->name[i].paths == NULL)
				 
				goto empty_set;

			if(match)
				 
				*new = add_subdir(*new, path->name[i].paths);
		}
	}

	if((*new)->count == 0) {
		 
		free_subdir(*new);
		*new = NULL;
		return FALSE;
	}

	 
	return TRUE;

empty_set:
	 
	free_subdir(*new);
	*new = NULL;
	return TRUE;
}

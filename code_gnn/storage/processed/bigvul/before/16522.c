process_directory( char* dirlist, char* host )
{
	StringList locals;
	Directory *files;
	const char *file, *dirpath;
	char **paths;
	int local_required;
	Regex excludeFilesRegex;
	
	local_required = param_boolean_crufty("REQUIRE_LOCAL_CONFIG_FILE", true);

	if(!dirlist) { return; }
	locals.initializeFromString( dirlist );
	locals.rewind();
	get_exclude_regex(excludeFilesRegex);
	while( (dirpath = locals.next()) ) {

		paths = (char **)calloc(65536, sizeof(char *));
		files = new Directory(dirpath);
		int i = 0;
		if(files == NULL) {
			fprintf(stderr, "Cannot open %s\n", dirpath);
		} else {
			while( (file = files->Next()) && i < 65536) {
				if(! files->IsDirectory() ) {
					if(!excludeFilesRegex.isInitialized() ||
					   !excludeFilesRegex.match(file)) {
						paths[i] = strdup(files->GetFullPath());
						i++;
					} else {
						dprintf(D_FULLDEBUG|D_CONFIG,
							"Ignoring config file "
							"based on "
							"LOCAL_CONFIG_DIR_EXCLUDE_REGEXP, "
							"'%s'\n", files->GetFullPath());
					}
				}
			}
			delete files;
		}
		qsort(paths, i, sizeof(char *), compareFiles);
		char **pathCopy = paths;
		while(*pathCopy) {
			process_config_source( *pathCopy, "config source", host,
								   local_required );

			local_config_sources.append(*pathCopy);

			free(*pathCopy);
			pathCopy++;
		}
		free(paths);
	}
}

xcursor_load_theme(const char *theme, int size,
		    void (*load_callback)(XcursorImages *, void *),
		    void *user_data)
{
	char *full, *dir;
	char *inherits = NULL;
	const char *path, *i;

	if (!theme)
		theme = "default";

	for (path = XcursorLibraryPath();
	     path;
	     path = _XcursorNextPath(path)) {
		dir = _XcursorBuildThemeDir(path, theme);
		if (!dir)
			continue;

		full = _XcursorBuildFullname(dir, "cursors", "");

		if (full) {
			load_all_cursors_from_dir(full, size, load_callback,
						  user_data);
			free(full);
		}

		if (!inherits) {
			full = _XcursorBuildFullname(dir, "", "index.theme");
			if (full) {
				inherits = _XcursorThemeInherits(full);
				free(full);
			}
		}

		free(dir);
	}

	for (i = inherits; i; i = _XcursorNextPath(i))
		xcursor_load_theme(i, size, load_callback, user_data);

	if (inherits)
		free(inherits);
}

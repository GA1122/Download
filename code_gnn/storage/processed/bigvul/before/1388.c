_XcursorBuildFullname (const char *dir, const char *subdir, const char *file)
{
    char    *full;

    if (!dir || !subdir || !file)
        return NULL;

    full = malloc (strlen (dir) + 1 + strlen (subdir) + 1 + strlen (file) + 1);
    if (!full)
	return NULL;
    full[0] = '\0';
    _XcursorAddPathElt (full, dir, -1);
    _XcursorAddPathElt (full, subdir, -1);
    _XcursorAddPathElt (full, file, -1);
    return full;
}

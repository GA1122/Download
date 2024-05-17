XcursorLibraryPath (void)
{
    static const char	*path;

    if (!path)
    {
	path = getenv ("XCURSOR_PATH");
	if (!path)
	    path = XCURSORPATH;
    }
    return path;
}

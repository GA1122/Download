_XcursorBuildThemeDir (const char *dir, const char *theme)
{
    const char	    *colon;
    const char	    *tcolon;
    char	    *full;
    char	    *home;
    int		    dirlen;
    int		    homelen;
    int		    themelen;
    int		    len;

    if (!dir || !theme)
        return NULL;

    colon = strchr (dir, ':');
    if (!colon)
	colon = dir + strlen (dir);

    dirlen = colon - dir;

    tcolon = strchr (theme, ':');
    if (!tcolon)
	tcolon = theme + strlen (theme);

    themelen = tcolon - theme;

    home = NULL;
    homelen = 0;
    if (*dir == '~')
    {
	home = getenv ("HOME");
	if (!home)
	    return NULL;
	homelen = strlen (home);
	dir++;
	dirlen--;
    }

     
    len = 1 + homelen + 1 + dirlen + 1 + themelen + 1;

    full = malloc (len);
    if (!full)
	return NULL;
    full[0] = '\0';

    if (home)
	_XcursorAddPathElt (full, home, -1);
    _XcursorAddPathElt (full, dir, dirlen);
    _XcursorAddPathElt (full, theme, themelen);
    return full;
}

_XcursorAddPathElt (char *path, const char *elt, int len)
{
    int	    pathlen = strlen (path);

     
    if (path[0] == '\0' || path[pathlen - 1] != '/')
    {
	strcat (path, "/");
	pathlen++;
    }
    if (len == -1)
	len = strlen (elt);
     
    while (len && elt[0] == '/')
    {
	elt++;
	len--;
    }
    strncpy (path + pathlen, elt, len);
    path[pathlen + len] = '\0';
}

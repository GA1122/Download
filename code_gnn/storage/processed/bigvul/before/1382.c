XcursorImagesSetName (XcursorImages *images, const char *name)
{
    char    *new;

    if (!images || !name)
        return;

    new = malloc (strlen (name) + 1);

    if (!new)
	return;

    strcpy (new, name);
    if (images->name)
	free (images->name);
    images->name = new;
}

XcursorImagesDestroy (XcursorImages *images)
{
    int	n;

    if (!images)
        return;

    for (n = 0; n < images->nimage; n++)
	XcursorImageDestroy (images->images[n]);
    if (images->name)
	free (images->name);
    free (images);
}

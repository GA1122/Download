XcursorImagesCreate (int size)
{
    XcursorImages   *images;

    images = malloc (sizeof (XcursorImages) +
		     size * sizeof (XcursorImage *));
    if (!images)
	return NULL;
    images->nimage = 0;
    images->images = (XcursorImage **) (images + 1);
    images->name = NULL;
    return images;
}

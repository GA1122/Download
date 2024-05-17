XcursorXcFileLoadImages (XcursorFile *file, int size)
{
    XcursorFileHeader	*fileHeader;
    XcursorDim		bestSize;
    int			nsize;
    XcursorImages	*images;
    int			n;
    int			toc;

    if (!file || size < 0)
	return NULL;
    fileHeader = _XcursorReadFileHeader (file);
    if (!fileHeader)
	return NULL;
    bestSize = _XcursorFindBestSize (fileHeader, (XcursorDim) size, &nsize);
    if (!bestSize)
    {
        _XcursorFileHeaderDestroy (fileHeader);
	return NULL;
    }
    images = XcursorImagesCreate (nsize);
    if (!images)
    {
        _XcursorFileHeaderDestroy (fileHeader);
	return NULL;
    }
    for (n = 0; n < nsize; n++)
    {
	toc = _XcursorFindImageToc (fileHeader, bestSize, n);
	if (toc < 0)
	    break;
	images->images[images->nimage] = _XcursorReadImage (file, fileHeader,
							    toc);
	if (!images->images[images->nimage])
	    break;
	images->nimage++;
    }
    _XcursorFileHeaderDestroy (fileHeader);
    if (images->nimage != nsize)
    {
	XcursorImagesDestroy (images);
	images = NULL;
    }
    return images;
}

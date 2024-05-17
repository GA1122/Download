_XcursorReadComment (XcursorFile	    *file,
		     XcursorFileHeader	    *fileHeader,
		     int		    toc)
{
    XcursorChunkHeader	chunkHeader;
    XcursorUInt		length;
    XcursorComment	*comment;

    if (!file || !fileHeader)
        return NULL;

     
    if (!_XcursorFileReadChunkHeader (file, fileHeader, toc, &chunkHeader))
	return NULL;
     
    if (!_XcursorReadUInt (file, &length))
	return NULL;
    comment = XcursorCommentCreate (chunkHeader.subtype, length);
    if (!comment)
	return NULL;
    if (!_XcursorReadBytes (file, comment->comment, length))
    {
	XcursorCommentDestroy (comment);
	return NULL;
    }
    comment->comment[length] = '\0';
    return comment;
}

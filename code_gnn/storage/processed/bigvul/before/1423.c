_XcursorWriteComment (XcursorFile	    *file,
		      XcursorFileHeader	    *fileHeader,
		      int		    toc,
		      XcursorComment	    *comment)
{
    XcursorChunkHeader	chunkHeader;
    XcursorUInt		length;

    if (!file || !fileHeader || !comment || !comment->comment)
        return XcursorFalse;

    length = strlen (comment->comment);

     
    if (length > XCURSOR_COMMENT_MAX_LEN)
	return XcursorFalse;

     
    chunkHeader.header = XCURSOR_COMMENT_HEADER_LEN;
    chunkHeader.type = XCURSOR_COMMENT_TYPE;
    chunkHeader.subtype = comment->comment_type;
    chunkHeader.version = XCURSOR_COMMENT_VERSION;

    if (!_XcursorFileWriteChunkHeader (file, fileHeader, toc, &chunkHeader))
	return XcursorFalse;

     
    if (!_XcursorWriteUInt (file, length))
	return XcursorFalse;

    if (!_XcursorWriteBytes (file, comment->comment, length))
	return XcursorFalse;
    return XcursorTrue;
}

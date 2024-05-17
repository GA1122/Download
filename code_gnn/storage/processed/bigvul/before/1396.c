_XcursorReadUInt (XcursorFile *file, XcursorUInt *u)
{
    unsigned char   bytes[4];

    if (!file || !u)
        return XcursorFalse;

    if ((*file->read) (file, bytes, 4) != 4)
	return XcursorFalse;
    *u = ((bytes[0] << 0) |
	  (bytes[1] << 8) |
	  (bytes[2] << 16) |
	  (bytes[3] << 24));
    return XcursorTrue;
}

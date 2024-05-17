zip_deflate_init(struct archive_read *a, struct zip *zip)
{
	int r;

	 
	if (!zip->decompress_init) {
		if (zip->stream_valid)
			r = inflateReset(&zip->stream);
		else
			r = inflateInit2(&zip->stream,
			    -15  );
		if (r != Z_OK) {
			archive_set_error(&a->archive, ARCHIVE_ERRNO_MISC,
			    "Can't initialize ZIP decompression.");
			return (ARCHIVE_FATAL);
		}
		 
		zip->stream_valid = 1;
		 
		zip->decompress_init = 1;
	}
	return (ARCHIVE_OK);
}

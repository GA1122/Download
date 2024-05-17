ssize_t rawexpwrite(off_t a, char *buf, size_t len, CLIENT *client, int fua) {
	int fhandle;
	off_t foffset;
	size_t maxbytes;
	ssize_t retval;

	if(get_filepos(client->export, a, &fhandle, &foffset, &maxbytes))
		return -1;
	if(maxbytes && len > maxbytes)
		len = maxbytes;

	DEBUG("(WRITE to fd %d offset %llu len %u fua %d), ", fhandle, (long long unsigned)foffset, (unsigned int)len, fua);

	myseek(fhandle, foffset);
	retval = write(fhandle, buf, len);
	if(client->server->flags & F_SYNC) {
		fsync(fhandle);
	} else if (fua) {

	   
#if 0
		sync_file_range(fhandle, foffset, len,
				SYNC_FILE_RANGE_WAIT_BEFORE | SYNC_FILE_RANGE_WRITE |
				SYNC_FILE_RANGE_WAIT_AFTER);
#else
		fdatasync(fhandle);
#endif
	}
	return retval;
}

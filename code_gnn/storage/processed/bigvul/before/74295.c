int read_block(int fd, long long start, long long *next, int expected,
								void *block)
{
	unsigned short c_byte;
	int offset = 2, res, compressed;
	int outlen = expected ? expected : SQUASHFS_METADATA_SIZE;
	
	if(swap) {
		if(read_fs_bytes(fd, start, 2, &c_byte) == FALSE)
			goto failed;
		c_byte = (c_byte >> 8) | ((c_byte & 0xff) << 8);
	} else 
		if(read_fs_bytes(fd, start, 2, &c_byte) == FALSE)
			goto failed;

	TRACE("read_block: block @0x%llx, %d %s bytes\n", start,
		SQUASHFS_COMPRESSED_SIZE(c_byte), SQUASHFS_COMPRESSED(c_byte) ?
		"compressed" : "uncompressed");

	if(SQUASHFS_CHECK_DATA(sBlk.s.flags))
		offset = 3;

	compressed = SQUASHFS_COMPRESSED(c_byte);
	c_byte = SQUASHFS_COMPRESSED_SIZE(c_byte);

	 
	if(c_byte > outlen)
		return 0;

	if(compressed) {
		char buffer[c_byte];
		int error;

		res = read_fs_bytes(fd, start + offset, c_byte, buffer);
		if(res == FALSE)
			goto failed;

		res = compressor_uncompress(comp, block, buffer, c_byte,
			outlen, &error);

		if(res == -1) {
			ERROR("%s uncompress failed with error code %d\n",
				comp->name, error);
			goto failed;
		}
	} else {
		res = read_fs_bytes(fd, start + offset, c_byte, block);
		if(res == FALSE)
			goto failed;
		res = c_byte;
	}

	if(next)
		*next = start + offset + c_byte;

	 
	if(expected && expected != res)
		return 0;
	else
		return res;

failed:
	ERROR("read_block: failed to read block @0x%llx\n", start);
	FAILED = TRUE;
	return FALSE;
}
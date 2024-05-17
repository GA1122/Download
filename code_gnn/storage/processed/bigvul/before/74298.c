int read_inode_table(long long start, long long end)
{
	int res;
	long long size = 0;
	long long bytes = 0;

	TRACE("read_inode_table: start %lld, end %lld\n", start, end);

	while(start < end) {
		if(size - bytes < SQUASHFS_METADATA_SIZE) {
			inode_table = realloc(inode_table, size +=
				SQUASHFS_METADATA_SIZE);
			if(inode_table == NULL) {
				ERROR("Out of memory in read_inode_table");
				goto failed;
			}
		}

		add_entry(inode_table_hash, start, bytes);

		res = read_block(fd, start, &start, 0, inode_table + bytes);
		if(res == 0) {
			ERROR("read_inode_table: failed to read block\n");
			goto failed;
		}
		bytes += res;

		 
		if(start != end && res != SQUASHFS_METADATA_SIZE) {
			ERROR("read_inode_table: metadata block should be %d "
				"bytes in length, it is %d bytes\n",
				SQUASHFS_METADATA_SIZE, res);
			
			goto failed;
		}
	}

	return TRUE;

failed:
	free(inode_table);
	FAILED = TRUE;
	return FALSE;
}
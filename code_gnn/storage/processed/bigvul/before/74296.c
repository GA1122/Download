int read_directory_table(long long start, long long end)
{
	int res;
	long long bytes = 0;
	long long size = 0;

	TRACE("read_directory_table: start %lld, end %lld\n", start, end);

	while(start < end) {
		if(size - bytes < SQUASHFS_METADATA_SIZE) {
			directory_table = realloc(directory_table, size +=
				SQUASHFS_METADATA_SIZE);
			if(directory_table == NULL) {
				ERROR("Out of memory in "
						"read_directory_table\n");
				goto failed;
			}
		}

		add_entry(directory_table_hash, start, bytes);

		res = read_block(fd, start, &start, 0, directory_table + bytes);
		if(res == 0) {
			ERROR("read_directory_table: failed to read block\n");
			goto failed;
		}

		bytes += res;

		 
		if(start != end && res != SQUASHFS_METADATA_SIZE) {
			ERROR("read_directory_table: metadata block "
				"should be %d bytes in length, it is %d "
				"bytes\n", SQUASHFS_METADATA_SIZE, res);
			goto failed;
		}
	}

	return TRUE;

failed:
	free(directory_table);
	FAILED = TRUE;
	return FALSE;
}

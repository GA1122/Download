int write_file(struct inode *inode, char *pathname)
{
	unsigned int file_fd, i;
	unsigned int *block_list;
	int file_end = inode->data / block_size;
	long long start = inode->start;

	TRACE("write_file: regular file, blocks %d\n", inode->blocks);

	file_fd = open_wait(pathname, O_CREAT | O_WRONLY |
		(force ? O_TRUNC : 0), (mode_t) inode->mode & 0777);
	if(file_fd == -1) {
		ERROR("write_file: failed to create file %s, because %s\n",
			pathname, strerror(errno));
		return FALSE;
	}

	block_list = malloc(inode->blocks * sizeof(unsigned int));
	if(block_list == NULL)
		EXIT_UNSQUASH("write_file: unable to malloc block list\n");

	s_ops.read_block_list(block_list, inode->block_ptr, inode->blocks);

	 
	queue_file(pathname, file_fd, inode);

	for(i = 0; i < inode->blocks; i++) {
		int c_byte = SQUASHFS_COMPRESSED_SIZE_BLOCK(block_list[i]);
		struct file_entry *block = malloc(sizeof(struct file_entry));

		if(block == NULL)
			EXIT_UNSQUASH("write_file: unable to malloc file\n");
		block->offset = 0;
		block->size = i == file_end ? inode->data & (block_size - 1) :
			block_size;
		if(block_list[i] == 0)  
			block->buffer = NULL;
		else {
			block->buffer = cache_get(data_cache, start,
				block_list[i]);
			start += c_byte;
		}
		queue_put(to_writer, block);
	}

	if(inode->frag_bytes) {
		int size;
		long long start;
		struct file_entry *block = malloc(sizeof(struct file_entry));

		if(block == NULL)
			EXIT_UNSQUASH("write_file: unable to malloc file\n");
		s_ops.read_fragment(inode->fragment, &start, &size);
		block->buffer = cache_get(fragment_cache, start, size);
		block->offset = inode->offset;
		block->size = inode->frag_bytes;
		queue_put(to_writer, block);
	}

	free(block_list);
	return TRUE;
}
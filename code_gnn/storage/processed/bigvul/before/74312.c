void *writer(void *arg)
{
	int i;

	while(1) {
		struct squashfs_file *file = queue_get(to_writer);
		int file_fd;
		long long hole = 0;
		int failed = FALSE;
		int error;

		if(file == NULL) {
			queue_put(from_writer, NULL);
			continue;
		} else if(file->fd == -1) {
			 
			set_attributes(file->pathname, file->mode, file->uid,
				file->gid, file->time, file->xattr, TRUE);
			free(file->pathname);
			free(file);
			continue;
		}

		TRACE("writer: regular file, blocks %d\n", file->blocks);

		file_fd = file->fd;

		for(i = 0; i < file->blocks; i++, cur_blocks ++) {
			struct file_entry *block = queue_get(to_writer);

			if(block->buffer == 0) {  
				hole += block->size;
				free(block);
				continue;
			}

			cache_block_wait(block->buffer);

			if(block->buffer->error)
				FAILED = failed = TRUE;

			if(failed)
				continue;

			error = write_block(file_fd, block->buffer->data +
				block->offset, block->size, hole, file->sparse);

			if(error == FALSE) {
				ERROR("writer: failed to write data block %d\n",
					i);
				FAILED = failed = TRUE;
			}

			hole = 0;
			cache_block_put(block->buffer);
			free(block);
		}

		if(hole && failed == FALSE) {
			 
			if(file->sparse == FALSE ||
					lseek(file_fd, hole, SEEK_CUR) == -1) {
				 
				hole --;
				if(write_block(file_fd, "\0", 1, hole,
						file->sparse) == FALSE) {
					ERROR("writer: failed to write sparse "
						"data block\n");
					FAILED = failed = TRUE;
				}
			} else if(ftruncate(file_fd, file->file_size) == -1) {
				ERROR("writer: failed to write sparse data "
					"block\n");
				FAILED = failed = TRUE;
			}
		}

		close_wake(file_fd);
		if(failed == FALSE)
			set_attributes(file->pathname, file->mode, file->uid,
				file->gid, file->time, file->xattr, force);
		else {
			ERROR("Failed to write %s, skipping\n", file->pathname);
			unlink(file->pathname);
		}
		free(file->pathname);
		free(file);

	}
}
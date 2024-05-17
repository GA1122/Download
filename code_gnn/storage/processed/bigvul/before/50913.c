zisofs_rewind_boot_file(struct archive_write *a)
{
	struct iso9660 *iso9660 = a->format_data;
	struct isofile *file;
	unsigned char *rbuff;
	ssize_t r;
	size_t remaining, rbuff_size;
	struct zisofs_extract zext;
	int64_t read_offset, write_offset, new_offset;
	int fd, ret = ARCHIVE_OK;

	file = iso9660->el_torito.boot->file;
	 
	if (file->zisofs.header_size == 0)
		return (ARCHIVE_OK);

	 
	memset(&zext, 0, sizeof(zext));
	zext.pz_uncompressed_size = file->zisofs.uncompressed_size;
	zext.pz_log2_bs = file->zisofs.log2_bs;

	fd = iso9660->temp_fd;
	new_offset = wb_offset(a);
	read_offset = file->content.offset_of_temp;
	remaining = (size_t)file->content.size;
	if (remaining > 1024 * 32)
		rbuff_size = 1024 * 32;
	else
		rbuff_size = remaining;

	rbuff = malloc(rbuff_size);
	if (rbuff == NULL) {
		archive_set_error(&a->archive, ENOMEM, "Can't allocate memory");
		return (ARCHIVE_FATAL);
	}
	while (remaining) {
		size_t rsize;
		ssize_t rs;

		 
		write_offset = lseek(fd, 0, SEEK_CUR);

		 
		lseek(fd, read_offset, SEEK_SET);

		rsize = rbuff_size;
		if (rsize > remaining)
			rsize = remaining;
		rs = read(iso9660->temp_fd, rbuff, rsize);
		if (rs <= 0) {
			archive_set_error(&a->archive, errno,
			    "Can't read temporary file(%jd)", (intmax_t)rs);
			ret = ARCHIVE_FATAL;
			break;
		}
		remaining -= rs;
		read_offset += rs;

		 
		lseek(fd, write_offset, SEEK_SET);

		r = zisofs_extract(a, &zext, rbuff, rs);
		if (r < 0) {
			ret = (int)r;
			break;
		}
	}

	if (ret == ARCHIVE_OK) {
		 
		file->content.offset_of_temp = new_offset;
		file->content.size = file->zisofs.uncompressed_size;
		archive_entry_set_size(file->entry, file->content.size);
		 
		file->zisofs.header_size = 0;
		file->zisofs.log2_bs = 0;
		file->zisofs.uncompressed_size = 0;
		r = wb_write_padding_to_temp(a, file->content.size);
		if (r < 0)
			ret = ARCHIVE_FATAL;
	}

	 
	free(rbuff);
	free(zext.block_pointers);
	if (zext.stream_valid && inflateEnd(&(zext.stream)) != Z_OK) {
        	archive_set_error(&a->archive, ARCHIVE_ERRNO_MISC,
		    "Failed to clean up compressor");
		ret = ARCHIVE_FATAL;
	}

	return (ret);
}

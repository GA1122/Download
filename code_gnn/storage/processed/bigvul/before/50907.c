zisofs_finish_entry(struct archive_write *a)
{
	struct iso9660 *iso9660 = a->format_data;
	struct isofile *file = iso9660->cur_file;
	unsigned char buff[16];
	size_t s;
	int64_t tail;

	 
	archive_entry_set_size(file->entry, iso9660->zisofs.total_size);

	 
	tail = wb_offset(a);

	 
	memcpy(buff, zisofs_magic, 8);
	set_num_731(buff+8, file->zisofs.uncompressed_size);
	buff[12] = file->zisofs.header_size;
	buff[13] = file->zisofs.log2_bs;
	buff[14] = buff[15] = 0; 

	 
	wb_set_offset(a, file->content.offset_of_temp);

	 
	if (wb_write_to_temp(a, buff, 16) != ARCHIVE_OK)
		return (ARCHIVE_FATAL);

	 
	s = iso9660->zisofs.block_pointers_cnt *
	    sizeof(iso9660->zisofs.block_pointers[0]);
	if (wb_write_to_temp(a, iso9660->zisofs.block_pointers, s)
	    != ARCHIVE_OK)
		return (ARCHIVE_FATAL);

	 
	wb_set_offset(a, tail);

	return (ARCHIVE_OK);
}

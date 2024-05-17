read_zip64_eocd(struct archive_read *a, struct zip *zip, const char *p)
{
	int64_t eocd64_offset;
	int64_t eocd64_size;

	 

	 
	if (archive_le32dec(p + 4) != 0)
		return;
	 
	if (archive_le32dec(p + 16) != 1)
		return;

	 
	eocd64_offset = archive_le64dec(p + 8);
	if (__archive_read_seek(a, eocd64_offset, SEEK_SET) < 0)
		return;
	if ((p = __archive_read_ahead(a, 56, NULL)) == NULL)
		return;
	 
	eocd64_size = archive_le64dec(p + 4) + 12;
	if (eocd64_size < 56 || eocd64_size > 16384)
		return;
	if ((p = __archive_read_ahead(a, (size_t)eocd64_size, NULL)) == NULL)
		return;

	 
	if (archive_le32dec(p + 16) != 0)  
		return;
	if (archive_le32dec(p + 20) != 0)  
		return;
	 
	if (archive_le64dec(p + 24) != archive_le64dec(p + 32))
		return;

	 
	zip->central_directory_offset = archive_le64dec(p + 48);
}

read_eocd(struct zip *zip, const char *p, int64_t current_offset)
{
	 

	 
	if (archive_le16dec(p + 4) != 0)
		return 0;
	 
	if (archive_le16dec(p + 4) != archive_le16dec(p + 6))
		return 0;
	 
	if (archive_le16dec(p + 10) != archive_le16dec(p + 8))
		return 0;
	 
	if (archive_le32dec(p + 16) + archive_le32dec(p + 12)
	    > current_offset)
		return 0;

	 
	zip->central_directory_offset = archive_le32dec(p + 16);

	 
	return 32;
}

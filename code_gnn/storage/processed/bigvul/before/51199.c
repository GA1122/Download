isJolietSVD(struct iso9660 *iso9660, const unsigned char *h)
{
	const unsigned char *p;
	ssize_t logical_block_size;
	int32_t volume_block;

	 
	if (!isSVD(iso9660, h))
		return (0);

	 

	 
	p = h + SVD_escape_sequences_offset;
	 
	if (p[0] == '%' && p[1] == '/') {
		int level = 0;

		if (p[2] == '@')
			level = 1;
		else if (p[2] == 'C')
			level = 2;
		else if (p[2] == 'E')
			level = 3;
		else  
			return (0);

		iso9660->seenJoliet = level;

	} else  
		return (0);

	logical_block_size =
	    archive_le16dec(h + SVD_logical_block_size_offset);
	volume_block = archive_le32dec(h + SVD_volume_space_size_offset);

	iso9660->logical_block_size = logical_block_size;
	iso9660->volume_block = volume_block;
	iso9660->volume_size = logical_block_size * (uint64_t)volume_block;
	 
	p = h + SVD_root_directory_record_offset;
	iso9660->joliet.location = archive_le32dec(p + DR_extent_offset);
	iso9660->joliet.size = archive_le32dec(p + DR_size_offset);

	return (48);
}

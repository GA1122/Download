isPVD(struct iso9660 *iso9660, const unsigned char *h)
{
	const unsigned char *p;
	ssize_t logical_block_size;
	int32_t volume_block;
	int32_t location;
	int i;

	 
	if (h[PVD_type_offset] != 1)
		return (0);

	 
	if (h[PVD_version_offset] != 1)
		return (0);

	 
	if (h[PVD_reserved1_offset] != 0)
		return (0);

	 
	if (!isNull(iso9660, h, PVD_reserved2_offset, PVD_reserved2_size))
		return (0);

	 
	if (!isNull(iso9660, h, PVD_reserved3_offset, PVD_reserved3_size))
		return (0);

	 
	 
	logical_block_size =
	    archive_le16dec(h + PVD_logical_block_size_offset);
	if (logical_block_size <= 0)
		return (0);

	volume_block = archive_le32dec(h + PVD_volume_space_size_offset);
	if (volume_block <= SYSTEM_AREA_BLOCK+4)
		return (0);

	 
	if (h[PVD_file_structure_version_offset] != 1)
		return (0);

	 
	location = archive_le32dec(h+PVD_type_1_path_table_offset);
	if (location < SYSTEM_AREA_BLOCK+2 || location >= volume_block)
		return (0);

	 
	location = archive_be32dec(h+PVD_type_m_path_table_offset);
	if ((location > 0 && location < SYSTEM_AREA_BLOCK+2)
	    || location >= volume_block)
		return (0);

	 
	 
	for (i = 0; i < PVD_reserved4_size; ++i)
		if (h[PVD_reserved4_offset + i] != 0
		    && h[PVD_reserved4_offset + i] != 0x20)
			return (0);

	 
	if (!isNull(iso9660, h, PVD_reserved5_offset, PVD_reserved5_size))
		return (0);

	 

	 
	p = h + PVD_root_directory_record_offset;
	if (p[DR_length_offset] != 34)
		return (0);

	if (!iso9660->primary.location) {
		iso9660->logical_block_size = logical_block_size;
		iso9660->volume_block = volume_block;
		iso9660->volume_size =
		    logical_block_size * (uint64_t)volume_block;
		iso9660->primary.location =
		    archive_le32dec(p + DR_extent_offset);
		iso9660->primary.size = archive_le32dec(p + DR_size_offset);
	}

	return (48);
}
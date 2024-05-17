int check_compression(struct compressor *comp)
{
	int res, bytes = 0;
	char buffer[SQUASHFS_METADATA_SIZE] __attribute__ ((aligned));

	if(!comp->supported) {
		ERROR("Filesystem uses %s compression, this is "
			"unsupported by this version\n", comp->name);
		ERROR("Decompressors available:\n");
		display_compressors("", "");
		return 0;
	}

	 
	if(SQUASHFS_COMP_OPTS(sBlk.s.flags)) {
		bytes = read_block(fd, sizeof(sBlk.s), NULL, 0, buffer);
		if(bytes == 0) {
			ERROR("Failed to read compressor options\n");
			return 0;
		}
	}

	res = compressor_check_options(comp, sBlk.s.block_size, buffer, bytes);

	return res != -1;
}

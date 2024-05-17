int PE_(bin_pe_get_actual_checksum)(struct PE_(r_bin_pe_obj_t)* bin) {
	int i, j, checksum_offset = 0;
	ut8* buf = NULL;
	ut64 computed_cs = 0;
	int remaining_bytes;
	int shift;
	ut32 cur;
	if (!bin || !bin->nt_header_offset) {
		return 0;
	}
	buf = bin->b->buf;
	checksum_offset = bin->nt_header_offset + 4 + sizeof(PE_(image_file_header)) + 0x40;
	for (i = 0; i < bin->size / 4; i++) {
		cur = r_read_le32 (&buf[i * 4]);

		if (i * 4 == checksum_offset) {
			continue;
		}

		computed_cs = (computed_cs & 0xFFFFFFFF) + cur + (computed_cs >> 32);
		if (computed_cs >> 32) {
			computed_cs = (computed_cs & 0xFFFFFFFF) + (computed_cs >> 32);
		}
	}

	remaining_bytes = bin->size % 4;
	i = i * 4;
	if (remaining_bytes != 0) {
		cur = buf[i];
		shift = 8;
		for (j = 1; j < remaining_bytes; j++, shift += 8) {
			cur |= buf[i + j] << shift;
		}
		computed_cs = (computed_cs & 0xFFFFFFFF) + cur + (computed_cs >> 32);
		if (computed_cs >> 32) {
			computed_cs = (computed_cs & 0xFFFFFFFF) + (computed_cs >> 32);
		}
	}

	computed_cs = (computed_cs & 0xFFFF) + (computed_cs >> 16);
	computed_cs = (computed_cs) + (computed_cs >> 16);
	computed_cs = (computed_cs & 0xFFFF);

	computed_cs += bin->size;
	return computed_cs;
}

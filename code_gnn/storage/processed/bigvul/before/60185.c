R_API bool r_bin_load_io_at_offset_as(RBin *bin, int fd, ut64 baseaddr,
		ut64 loadaddr, int xtr_idx, ut64 offset, const char *name) {
	const ut64 MAX_LOAD_SIZE = 0;   
	int res = r_bin_load_io_at_offset_as_sz (bin, fd, baseaddr,
		loadaddr, xtr_idx, offset, name, MAX_LOAD_SIZE);
	if (!res) {
		res = r_bin_load_io_at_offset_as_sz (bin, fd, baseaddr,
			loadaddr, xtr_idx, offset, name, UT64_MAX);
	}
	return res;
}

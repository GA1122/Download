struct MACH0_(obj_t)* MACH0_(new_buf)(RBuffer *buf, bool verbose) {
	if (!buf) {
		return NULL;
	}

	RBuffer * buf_copy = r_buf_new_with_buf (buf);
	if (!buf_copy) {
		return NULL;
	}

	return MACH0_(new_buf_steal) (buf_copy, verbose);
}

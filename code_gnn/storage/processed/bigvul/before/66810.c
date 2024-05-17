struct MACH0_(obj_t)* MACH0_(new_buf)(RBuffer *buf, bool verbose) {
	struct MACH0_(obj_t) *bin = R_NEW0 (struct MACH0_(obj_t));
	if (!bin) {
		return NULL;
	}
	bin->kv = sdb_new (NULL, "bin.mach0", 0);
	bin->b = r_buf_new ();
	bin->size = buf->length;
	bin->verbose = verbose;
	if (!r_buf_set_bytes (bin->b, buf->buf, bin->size)){
		return MACH0_(mach0_free) (bin);
	}
	if (!init (bin)) {
		return MACH0_(mach0_free)(bin);
	}
	return bin;
}

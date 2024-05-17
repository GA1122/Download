static void * load_bytes(RBinFile *bf, const ut8 *buf, ut64 sz, ut64 loadaddr, Sdb *sdb) {
	struct Elf_(r_bin_elf_obj_t) *res;
	if (!buf || !sz || sz == UT64_MAX) {
		return NULL;
	}
	RBuffer *tbuf = r_buf_new ();
	r_buf_set_bytes (tbuf, buf, sz);
	res = Elf_(r_bin_elf_new_buf) (tbuf, bf->rbin->verbose);
	if (res) {
		sdb_ns_set (sdb, "info", res->kv);
	}
	r_buf_free (tbuf);
	return res;
}
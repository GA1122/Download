char* MACH0_(get_cputype)(struct MACH0_(obj_t)* bin) {
	if (bin) {
		return MACH0_(get_cputype_from_hdr) (&bin->hdr);
	}
	return strdup ("unknown");
}

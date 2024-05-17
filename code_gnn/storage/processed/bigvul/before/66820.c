char* MACH0_(get_filetype)(struct MACH0_(obj_t)* bin) {
	if (bin) {
		return MACH0_(get_filetype_from_hdr) (&bin->hdr);
	}
	return strdup ("Unknown");
}

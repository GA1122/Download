void* MACH0_(mach0_free)(struct MACH0_(obj_t)* bin) {
	if (!bin) {
		return NULL;
	}
	free (bin->segs);
	free (bin->sects);
	free (bin->symtab);
	free (bin->symstr);
	free (bin->indirectsyms);
	free (bin->imports_by_ord);
	free (bin->dyld_info);
	free (bin->toc);
	free (bin->modtab);
	free (bin->libs);
	free (bin->func_start);
	free (bin->signature);
	r_buf_free (bin->b);
	free (bin);
	return NULL;
}

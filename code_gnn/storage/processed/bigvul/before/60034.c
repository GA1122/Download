static size_t get_relocs_num(ELFOBJ *bin) {
	size_t i, size, ret = 0;
	 
#define NUMENTRIES_ROUNDUP(sectionsize, entrysize) (((sectionsize)+(entrysize)-1)/(entrysize))
	if (!bin->g_sections) {
		return 0;
	}
	size = bin->is_rela == DT_REL ? sizeof (Elf_(Rel)) : sizeof (Elf_(Rela));
	for (i = 0; !bin->g_sections[i].last; i++) {
		if (!strncmp (bin->g_sections[i].name, ".rela.", strlen (".rela."))) {
			if (!bin->is_rela) {
				size = sizeof (Elf_(Rela));
			}
			ret += NUMENTRIES_ROUNDUP (bin->g_sections[i].size, size);
		} else if (!strncmp (bin->g_sections[i].name, ".rel.", strlen (".rel."))){
			if (!bin->is_rela) {
				size = sizeof (Elf_(Rel));
			}
			ret += NUMENTRIES_ROUNDUP (bin->g_sections[i].size, size);
		}
	}
	return ret;
#undef NUMENTRIES_ROUNDUP
}

static RList* sections(RBinFile *bf) {
	struct Elf_(r_bin_elf_obj_t)* obj = (bf && bf->o)? bf->o->bin_obj : NULL;
	struct r_bin_elf_section_t *section = NULL;
	int i, num, found_load = 0;
	Elf_(Phdr)* phdr = NULL;
	RBinSection *ptr = NULL;
	RList *ret = NULL;

	if (!obj || !(ret = r_list_newf (free))) {
		return NULL;
	}
	if ((section = Elf_(r_bin_elf_get_sections) (obj))) {
		for (i = 0; !section[i].last; i++) {
			if (!(ptr = R_NEW0 (RBinSection))) {
				break;
			}
			strncpy (ptr->name, (char*)section[i].name, R_BIN_SIZEOF_STRINGS);
			if (strstr (ptr->name, "data") && !strstr (ptr->name, "rel")) {
				ptr->is_data = true;
			}
			ptr->size = section[i].type != SHT_NOBITS ? section[i].size : 0;
			ptr->vsize = section[i].size;
			ptr->paddr = section[i].offset;
			ptr->vaddr = section[i].rva;
			ptr->add = !obj->phdr;  
			ptr->srwx = 0;
			if (R_BIN_ELF_SCN_IS_EXECUTABLE (section[i].flags)) {
				ptr->srwx |= R_BIN_SCN_EXECUTABLE;
			}
			if (R_BIN_ELF_SCN_IS_WRITABLE (section[i].flags)) {
				ptr->srwx |= R_BIN_SCN_WRITABLE;
			}
			if (R_BIN_ELF_SCN_IS_READABLE (section[i].flags)) {
				ptr->srwx |= R_BIN_SCN_READABLE;
			}
			r_list_append (ret, ptr);
		}
	}

	num = obj->ehdr.e_phnum;
	phdr = obj->phdr;
	if (phdr) {
		int n = 0;
		for (i = 0; i < num; i++) {
			if (!(ptr = R_NEW0 (RBinSection))) {
				return ret;
			}
			ptr->add = false;
			ptr->size = phdr[i].p_filesz;
			ptr->vsize = phdr[i].p_memsz;
			ptr->paddr = phdr[i].p_offset;
			ptr->vaddr = phdr[i].p_vaddr;
			ptr->srwx = phdr[i].p_flags;
			switch (phdr[i].p_type) {
			case PT_DYNAMIC:
				strncpy (ptr->name, "DYNAMIC", R_BIN_SIZEOF_STRINGS);
				break;
			case PT_LOAD:
				snprintf (ptr->name, R_BIN_SIZEOF_STRINGS, "LOAD%d", n++);
				found_load = 1;
				ptr->add = true;
				break;
			case PT_INTERP:
				strncpy (ptr->name, "INTERP", R_BIN_SIZEOF_STRINGS);
				break;
			case PT_GNU_STACK:
				strncpy (ptr->name, "GNU_STACK", R_BIN_SIZEOF_STRINGS);
				break;
			case PT_GNU_RELRO:
				strncpy (ptr->name, "GNU_RELRO", R_BIN_SIZEOF_STRINGS);
				break;
			case PT_GNU_EH_FRAME:
				strncpy (ptr->name, "GNU_EH_FRAME", R_BIN_SIZEOF_STRINGS);
				break;
			case PT_PHDR:
				strncpy (ptr->name, "PHDR", R_BIN_SIZEOF_STRINGS);
				break;
			case PT_TLS:
				strncpy (ptr->name, "TLS", R_BIN_SIZEOF_STRINGS);
				break;
			case PT_NOTE:
				strncpy (ptr->name, "NOTE", R_BIN_SIZEOF_STRINGS);
				break;
			default:
				strncpy (ptr->name, "UNKNOWN", R_BIN_SIZEOF_STRINGS);
				break;
			}
			ptr->name[R_BIN_SIZEOF_STRINGS - 1] = '\0';
			r_list_append (ret, ptr);
		}
	}

	if (r_list_empty (ret)) {
		if (!bf->size) {
			struct Elf_(r_bin_elf_obj_t) *bin = bf->o->bin_obj;
			bf->size = bin? bin->size: 0x9999;
		}
		if (found_load == 0) {
			if (!(ptr = R_NEW0 (RBinSection))) {
				return ret;
			}
			sprintf (ptr->name, "uphdr");
			ptr->size = bf->size;
			ptr->vsize = bf->size;
			ptr->paddr = 0;
			ptr->vaddr = 0x10000;
			ptr->add = true;
			ptr->srwx = R_BIN_SCN_READABLE | R_BIN_SCN_WRITABLE |
				R_BIN_SCN_EXECUTABLE;
			r_list_append (ret, ptr);
		}
	}
	ptr = R_NEW0 (RBinSection);
	if (ptr) {
		ut64 ehdr_size = sizeof (obj->ehdr);
		if (bf->size < ehdr_size) {
			ehdr_size = bf->size;
		}
		sprintf (ptr->name, "ehdr");
		ptr->paddr = 0;
		ptr->vaddr = obj->baddr;
		ptr->size = ehdr_size;
		ptr->vsize = ehdr_size;
		ptr->add = false;
		if (obj->ehdr.e_type == ET_REL) {
			ptr->add = true;
		}
		ptr->srwx = R_BIN_SCN_READABLE | R_BIN_SCN_WRITABLE;
		r_list_append (ret, ptr);
	}
	return ret;
}
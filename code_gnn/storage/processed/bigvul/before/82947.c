static RList* symbols(RBinFile *bf) {
	struct Elf_(r_bin_elf_obj_t) *bin;
	struct r_bin_elf_symbol_t *symbol = NULL;
	RBinSymbol *ptr = NULL;
	RList *ret = NULL;
	int i;

	if (!bf|| !bf->o || !bf->o->bin_obj) {
		return NULL;
	}

	bin = bf->o->bin_obj;
	ret = r_list_newf (free);
	if (!ret) {
		return NULL;
	}
	if (!(symbol = Elf_(r_bin_elf_get_symbols) (bin))) {
		return ret;
	}
	for (i = 0; !symbol[i].last; i++) {
		ut64 paddr = symbol[i].offset;
		ut64 vaddr = Elf_(r_bin_elf_p2v) (bin, paddr);
		if (!(ptr = R_NEW0 (RBinSymbol))) {
			break;
		}
		ptr->name = strdup (symbol[i].name);
		ptr->forwarder = r_str_const ("NONE");
		ptr->bind = r_str_const (symbol[i].bind);
		ptr->type = r_str_const (symbol[i].type);
		ptr->paddr = paddr;
		ptr->vaddr = vaddr;
		ptr->size = symbol[i].size;
		ptr->ordinal = symbol[i].ordinal;
		setsymord (bin, ptr->ordinal, ptr);
		if (bin->ehdr.e_machine == EM_ARM && *ptr->name) {
			_set_arm_thumb_bits (bin, &ptr);
		}
		r_list_append (ret, ptr);
	}
	if (!(symbol = Elf_(r_bin_elf_get_imports) (bin))) {
		return ret;
	}
	for (i = 0; !symbol[i].last; i++) {
		ut64 paddr = symbol[i].offset;
		ut64 vaddr = Elf_(r_bin_elf_p2v) (bin, paddr);
		if (!symbol[i].size) {
			continue;
		}
		if (!(ptr = R_NEW0 (RBinSymbol))) {
			break;
		}
		ptr->name = r_str_newf ("imp.%s", symbol[i].name);
		ptr->forwarder = r_str_const ("NONE");
		ptr->bind = r_str_const (symbol[i].bind);
		ptr->type = r_str_const (symbol[i].type);
		ptr->paddr = paddr;
		ptr->vaddr = vaddr;
		if (ptr->vaddr == UT32_MAX) {
			ptr->paddr = 0;
			ptr->vaddr = 0;
		}
		ptr->size = symbol[i].size;
		ptr->ordinal = symbol[i].ordinal;
		setsymord (bin, ptr->ordinal, ptr);
		 
		if (bin->ehdr.e_machine == EM_ARM) {
			_set_arm_thumb_bits (bin, &ptr);
		}
		r_list_append (ret, ptr);
	}
	return ret;
}
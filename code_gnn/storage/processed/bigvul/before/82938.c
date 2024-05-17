static RList* patch_relocs(RBin *b) {
	RList *ret = NULL;
	RBinReloc *ptr = NULL;
	RIO *io = NULL;
	RBinObject *obj = NULL;
	struct Elf_(r_bin_elf_obj_t) *bin = NULL;
	RIOSection *g = NULL, *s = NULL;
	SdbHash *relocs_by_sym;
	SdbListIter *iter;
	RBinElfReloc *relcs = NULL;
	RBinInfo *info;
	int cdsz;
	int i;
	ut64 n_off, n_vaddr, vaddr, size, offset = 0;

	if (!b)
		return NULL;
	io = b->iob.io;
	if (!io || !io->desc)
		return NULL;
	obj = r_bin_cur_object (b);
	if (!obj) {
	   	return NULL;
	}
	bin = obj->bin_obj;
	if (bin->ehdr.e_type != ET_REL) {
		return NULL;
	}
	if (!io->cached) {
	   	eprintf ("Warning: run r2 with -e io.cache=true to fix relocations in disassembly\n");
		return relocs (r_bin_cur (b));
	}

	info = obj ? obj->info: NULL;
	cdsz = info? (info->bits == 64? 8: info->bits == 32? 4: info->bits == 16 ? 4: 0): 0;

	ls_foreach (io->sections, iter, s) {
		if (s->paddr > offset) {
			offset = s->paddr;
			g = s;
		}
	}
	if (!g) {
		return NULL;
	}
	n_off = g->paddr + g->size;
	n_vaddr = g->vaddr + g->vsize;
	size = bin->reloc_num * 4;
	if (!b->iob.section_add (io, n_off, n_vaddr, size, size, R_BIN_SCN_READABLE, ".got.r2", 0, io->desc->fd)) {
		return NULL;
	}
	if (!(relcs = Elf_(r_bin_elf_get_relocs) (bin))) {
		return NULL;
	}
	if (!(ret = r_list_newf ((RListFree)free))) {
		free (relcs);
		return NULL;
	}
	if (!(relocs_by_sym = ht_new (NULL, relocs_by_sym_free, NULL))) {
		r_list_free (ret);
		free (relcs);
		return NULL;
	}
	vaddr = n_vaddr;
	for (i = 0; !relcs[i].last; i++) {
		ut64 sym_addr = 0;

		if (relcs[i].sym) {
			if (relcs[i].sym < bin->imports_by_ord_size && bin->imports_by_ord[relcs[i].sym]) {
				bool found;

				sym_addr = ht_find_intu64 (relocs_by_sym, relcs[i].sym, &found);
				if (!found) {
					sym_addr = 0;
				}
			} else if (relcs[i].sym < bin->symbols_by_ord_size && bin->symbols_by_ord[relcs[i].sym]) {
				sym_addr = bin->symbols_by_ord[relcs[i].sym]->vaddr;
			}
		}
		_patch_reloc (bin->ehdr.e_machine, &b->iob, &relcs[i], sym_addr ? sym_addr : vaddr, 0, n_vaddr + size);
		if (!(ptr = reloc_convert (bin, &relcs[i], n_vaddr))) {
			continue;
		}

		if (sym_addr) {
			ptr->vaddr = sym_addr;
		} else {
			ptr->vaddr = vaddr;
			ht_insert_intu64 (relocs_by_sym, relcs[i].sym, vaddr);
			vaddr += cdsz;
		}
		r_list_append (ret, ptr);
	}
	ht_free (relocs_by_sym);
	free (relcs);
	return ret;
}
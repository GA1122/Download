static int Elf_(fix_symbols)(ELFOBJ *bin, int nsym, int type, RBinElfSymbol **sym) {
	int count = 0;
	RBinElfSymbol *ret = *sym;
	RBinElfSymbol *phdr_symbols = (type == R_BIN_ELF_SYMBOLS)
				? Elf_(r_bin_elf_get_phdr_symbols) (bin)
				: Elf_(r_bin_elf_get_phdr_imports) (bin);
	RBinElfSymbol *tmp, *p;
	if (phdr_symbols) {
		RBinElfSymbol *d = ret;
		while (!d->last) {
			 
			p = phdr_symbols;
			while (!p->last) {
				if (p->offset && d->offset == p->offset) {
					p->in_shdr = true;
					if (*p->name && strcmp (d->name, p->name)) {
						strcpy (d->name, p->name);
					}
				}
				p++;
			}
			d++;
		}
		p = phdr_symbols;
		while (!p->last) {
			if (!p->in_shdr) {
				count++;
			}
			p++;
		}
		 
		 
		if (count > 0) {
			 
			tmp = (RBinElfSymbol*)realloc (ret, (nsym + count + 1) * sizeof (RBinElfSymbol));
			if (!tmp) {
				return -1;
			}
			ret = tmp;
			ret[nsym--].last = 0;
			p = phdr_symbols;
			while (!p->last) {
				if (!p->in_shdr) {
					memcpy (&ret[++nsym], p, sizeof (RBinElfSymbol));
				}
				p++;
			}
			ret[nsym + 1].last = 1;
		}
		*sym = ret;
		return nsym + 1;
	}
	return nsym;
}

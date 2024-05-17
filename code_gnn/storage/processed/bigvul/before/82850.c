static int parse_symtab(struct MACH0_(obj_t)* bin, ut64 off) {
	struct symtab_command st;
	ut32 size_sym;
	int i;
	ut8 symt[sizeof (struct symtab_command)] = {0};
	ut8 nlst[sizeof (struct MACH0_(nlist))] = {0};

	if (off > (ut64)bin->size || off + sizeof (struct symtab_command) > (ut64)bin->size) {
		return false;
	}
	int len = r_buf_read_at (bin->b, off, symt, sizeof (struct symtab_command));
	if (len != sizeof (struct symtab_command)) {
		bprintf ("Error: read (symtab)\n");
		return false;
	}
	st.cmd = r_read_ble32 (&symt[0], bin->big_endian);
	st.cmdsize = r_read_ble32 (&symt[4], bin->big_endian);
	st.symoff = r_read_ble32 (&symt[8], bin->big_endian);
	st.nsyms = r_read_ble32 (&symt[12], bin->big_endian);
	st.stroff = r_read_ble32 (&symt[16], bin->big_endian);
	st.strsize = r_read_ble32 (&symt[20], bin->big_endian);

	bin->symtab = NULL;
	bin->nsymtab = 0;
	if (st.strsize > 0 && st.strsize < bin->size && st.nsyms > 0) {
		bin->nsymtab = st.nsyms;
		if (st.stroff > bin->size || st.stroff + st.strsize > bin->size) {
			return false;
		}
		if (!UT32_MUL (&size_sym, bin->nsymtab, sizeof (struct MACH0_(nlist)))) {
			bprintf("fail2\n");
			return false;
		}
		if (!size_sym) {
			bprintf("fail3\n");
			return false;
		}
		if (st.symoff > bin->size || st.symoff + size_sym > bin->size) {
			bprintf("fail4\n");
			return false;
		}
		if (!(bin->symstr = calloc (1, st.strsize + 2))) {
			perror ("calloc (symstr)");
			return false;
		}
		bin->symstrlen = st.strsize;
		len = r_buf_read_at (bin->b, st.stroff, (ut8*)bin->symstr, st.strsize);
		if (len != st.strsize) {
			bprintf ("Error: read (symstr)\n");
			R_FREE (bin->symstr);
			return false;
		}
		if (!(bin->symtab = calloc (bin->nsymtab, sizeof (struct MACH0_(nlist))))) {
			perror ("calloc (symtab)");
			return false;
		}
		for (i = 0; i < bin->nsymtab; i++) {
			len = r_buf_read_at (bin->b, st.symoff + (i * sizeof (struct MACH0_(nlist))),
								nlst, sizeof (struct MACH0_(nlist)));
			if (len != sizeof (struct MACH0_(nlist))) {
				bprintf ("Error: read (nlist)\n");
				R_FREE (bin->symtab);
				return false;
			}
			bin->symtab[i].n_strx = r_read_ble32 (&nlst[0], bin->big_endian);
			bin->symtab[i].n_type = r_read_ble8 (&nlst[4]);
			bin->symtab[i].n_sect = r_read_ble8 (&nlst[5]);
			bin->symtab[i].n_desc = r_read_ble16 (&nlst[6], bin->big_endian);
#if R_BIN_MACH064
			bin->symtab[i].n_value = r_read_ble64 (&nlst[8], bin->big_endian);
#else
			bin->symtab[i].n_value = r_read_ble32 (&nlst[8], bin->big_endian);
#endif
		}
	}
	return true;
}
static int bin_fields(RCore *r, int mode, int va) {
	RList *fields;
	RListIter *iter;
	RBinField *field;
	int i = 0;
	RBin *bin = r->bin;
	RBinFile *binfile = r_core_bin_cur (r);
	ut64 size = binfile ? binfile->size : UT64_MAX;
	ut64 baddr = r_bin_get_baddr (r->bin);

	if (!(fields = r_bin_get_fields (bin))) {
		return false;
	}
	if (IS_MODE_JSON (mode)) {
		r_cons_print ("[");
	} else if (IS_MODE_RAD (mode)) {
		r_cons_println ("fs header");
	} else if (IS_MODE_NORMAL (mode)) {
		r_cons_println ("[Header fields]");
	}
#if 0
	else if (IS_MODE_SET (mode)) {
		int fd = r_core_file_cur_fd(r);
		r_io_section_add (r->io, 0, baddr, size, size, 7, "ehdr", 0, fd);
	}
#endif
	r_list_foreach (fields, iter, field) {
		ut64 addr = rva (bin, field->paddr, field->vaddr, va);

		if (IS_MODE_RAD (mode)) {
			r_name_filter (field->name, -1);
			r_cons_printf ("f header.%s @ 0x%08"PFMT64x"\n", field->name, addr);
			if (field->comment && *field->comment) {
				r_cons_printf ("CC %s @ 0x%"PFMT64x"\n", field->comment, addr);
			}
			if (field->format && *field->format) {
				r_cons_printf ("pf.%s %s\n", field->name, field->format);
			}
		} else if (IS_MODE_JSON (mode)) {
			r_cons_printf ("%s{\"name\":\"%s\","
				"\"vaddr\":%"PFMT64d","
				"\"paddr\":%"PFMT64d,
				iter->p? ",": "",
				field->name,
				field->vaddr,
				field->paddr
				);
			if (field->comment && *field->comment) {
				r_cons_printf (",\"comment\":\"%s\"", field->comment);
			}
			if (field->format && *field->format) {
				r_cons_printf (",\"format\":\"%s\"", field->format);
			}
			r_cons_printf ("}");
		} else if (IS_MODE_NORMAL (mode)) {
			const bool haveComment = (field->comment && *field->comment);
			r_cons_printf ("0x%08"PFMT64x" 0x%08"PFMT64x" %s%s%s\n",
				field->vaddr, field->paddr, field->name,
				haveComment? "; ": "",
				haveComment? field->comment: "");
		}
		i++;
	}
	if (IS_MODE_JSON (mode)) {
		r_cons_printf ("]");
	} else if (IS_MODE_RAD (mode)) {
		 
		r_cons_printf ("S 0 0x%"PFMT64x" 0x%"PFMT64x" 0x%"PFMT64x" ehdr rwx\n",
			baddr, size, size);
	} else if (IS_MODE_NORMAL (mode)) {
		r_cons_printf ("\n%i fields\n", i);
	}

	return true;
}
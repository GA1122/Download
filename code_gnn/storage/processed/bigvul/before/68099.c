static int getoffset(RBinFile *arch, int type, int idx) {
	struct r_bin_dex_obj_t *dex = arch->o->bin_obj;
	switch (type) {
	case 'm':  
		return offset_of_method_idx (arch, dex, idx);
	case 'o':  
		break;
	case 's':  
		if (dex->header.strings_size > idx) {
			if (dex->strings) return dex->strings[idx];
		}
		break;
	case 't':  
		return dex_get_type_offset (arch, idx);
	case 'c':  
		return dex_get_type_offset (arch, idx);
	}
	return -1;
}

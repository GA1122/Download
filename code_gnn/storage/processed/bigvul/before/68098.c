static char *getname(RBinFile *arch, int type, int idx) {
	struct r_bin_dex_obj_t *dex = arch->o->bin_obj;
	switch (type) {
	case 'm':  
		return dex_method_fullname (dex, idx);
	case 'c':  
		return dex_class_name_byid (dex, idx);
	case 'f':  
		return dex_field_name (dex, idx);
	}
	return NULL;
}
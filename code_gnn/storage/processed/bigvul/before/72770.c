int jas_image_fmtfromname(char *name)
{
	int i;
	char *ext;
	jas_image_fmtinfo_t *fmtinfo;
	 
	if (!(ext = strrchr(name, '.'))) {
		return -1;
	}
	++ext;
	 	
	for (i = 0, fmtinfo = jas_image_fmtinfos; i < jas_image_numfmts; ++i,
	  ++fmtinfo) {
		 
		if (!strcmp(ext, fmtinfo->ext)) {
			return fmtinfo->id;
		}
	}
	return -1;
}

lt_int_dyld_match_loaded_lib_by_install_name(const char *name)
{
	int i=_dyld_image_count();
	int j;
	const struct mach_header *mh=NULL;
	const char *id=NULL;
	for (j = 0; j < i; j++)
	{
		id=lt_int_dyld_lib_install_name(_dyld_get_image_header(j));
		if ((id) && (!strcmp(id,name)))
		{
			mh=_dyld_get_image_header(j);
			break;
		}
	}
	return mh;
}

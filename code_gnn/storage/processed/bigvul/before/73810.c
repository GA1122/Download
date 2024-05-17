static UPNP_INLINE int search_extension(
	 
	const char *extension,
	 
	const char **con_type,
	 
	const char **con_subtype)
{
	int top, mid, bot;
	int cmp;

	top = 0;
	bot = NUM_MEDIA_TYPES - 1;

	while (top <= bot) {
		mid = (top + bot) / 2;
		cmp = strcasecmp(extension, gMediaTypeList[mid].file_ext);
		if (cmp > 0) {
			 
			top = mid + 1;
		} else if (cmp < 0) {
			 
			bot = mid - 1;
		} else {
			 
			*con_type = gMediaTypeList[mid].content_type;
			*con_subtype = gMediaTypeList[mid].content_subtype;
			return 0;
		}
	}

	return -1;
}

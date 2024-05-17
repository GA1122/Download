static int exif_file_sections_realloc(image_info_type *ImageInfo, int section_index, size_t size TSRMLS_DC)
{
	void *tmp;

	 
	if (section_index >= ImageInfo->file.count) {
		EXIF_ERRLOG_FSREALLOC(ImageInfo)
		return -1;
	}
	tmp = safe_erealloc(ImageInfo->file.list[section_index].data, 1, size, 0);
	ImageInfo->file.list[section_index].data = tmp;
	ImageInfo->file.list[section_index].size = size;
	return 0;
}

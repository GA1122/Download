super_1_allow_new_offset(struct md_rdev *rdev,
			 unsigned long long new_offset)
{
	 
	struct bitmap *bitmap;
	if (new_offset >= rdev->data_offset)
		return 1;

	 
	if (rdev->mddev->minor_version == 0)
		return 1;

	 
	if (rdev->sb_start + (32+4)*2 > new_offset)
		return 0;
	bitmap = rdev->mddev->bitmap;
	if (bitmap && !rdev->mddev->bitmap_info.file &&
	    rdev->sb_start + rdev->mddev->bitmap_info.offset +
	    bitmap->storage.file_pages * (PAGE_SIZE>>9) > new_offset)
		return 0;
	if (rdev->badblocks.sector + rdev->badblocks.size > new_offset)
		return 0;

	return 1;
}

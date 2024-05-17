bitmap_store(struct mddev *mddev, const char *buf, size_t len)
{
	char *end;
	unsigned long chunk, end_chunk;
	int err;

	err = mddev_lock(mddev);
	if (err)
		return err;
	if (!mddev->bitmap)
		goto out;
	 
	while (*buf) {
		chunk = end_chunk = simple_strtoul(buf, &end, 0);
		if (buf == end) break;
		if (*end == '-') {  
			buf = end + 1;
			end_chunk = simple_strtoul(buf, &end, 0);
			if (buf == end) break;
		}
		if (*end && !isspace(*end)) break;
		bitmap_dirty_bits(mddev->bitmap, chunk, end_chunk);
		buf = skip_spaces(end);
	}
	bitmap_unplug(mddev->bitmap);  
out:
	mddev_unlock(mddev);
	return len;
}

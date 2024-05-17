static int add_to_include_set(struct include_data *data,
			      const unsigned char *sha1,
			      int bitmap_pos)
{
	khiter_t hash_pos;

	if (data->seen && bitmap_get(data->seen, bitmap_pos))
		return 0;

	if (bitmap_get(data->base, bitmap_pos))
		return 0;

	hash_pos = kh_get_sha1(bitmap_git.bitmaps, sha1);
	if (hash_pos < kh_end(bitmap_git.bitmaps)) {
		struct stored_bitmap *st = kh_value(bitmap_git.bitmaps, hash_pos);
		bitmap_or_ewah(data->base, lookup_stored_bitmap(st));
		return 0;
	}

	bitmap_set(data->base, bitmap_pos);
	return 1;
}

static void corrupt_bio_data(struct bio *bio, struct flakey_c *fc)
{
	unsigned bio_bytes = bio_cur_bytes(bio);
	char *data = bio_data(bio);

	 
	if (data && bio_bytes >= fc->corrupt_bio_byte) {
		data[fc->corrupt_bio_byte - 1] = fc->corrupt_bio_value;

		DMDEBUG("Corrupting data bio=%p by writing %u to byte %u "
			"(rw=%c bi_rw=%lu bi_sector=%llu cur_bytes=%u)\n",
			bio, fc->corrupt_bio_value, fc->corrupt_bio_byte,
			(bio_data_dir(bio) == WRITE) ? 'w' : 'r',
			bio->bi_rw, (unsigned long long)bio->bi_sector, bio_bytes);
	}
}
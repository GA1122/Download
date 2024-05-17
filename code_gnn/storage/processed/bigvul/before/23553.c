static int flakey_map(struct dm_target *ti, struct bio *bio,
		      union map_info *map_context)
{
	struct flakey_c *fc = ti->private;
	unsigned elapsed;

	 
	elapsed = (jiffies - fc->start_time) / HZ;
	if (elapsed % (fc->up_interval + fc->down_interval) >= fc->up_interval) {
		 
		map_context->ll = 1;

		 
		if (bio_data_dir(bio) == READ)
			goto map_bio;

		 
		if (test_bit(DROP_WRITES, &fc->flags)) {
			bio_endio(bio, 0);
			return DM_MAPIO_SUBMITTED;
		}

		 
		if (fc->corrupt_bio_byte && (fc->corrupt_bio_rw == WRITE)) {
			if (all_corrupt_bio_flags_match(bio, fc))
				corrupt_bio_data(bio, fc);
			goto map_bio;
		}

		 
		return -EIO;
	}

map_bio:
	flakey_map_bio(ti, bio);

	return DM_MAPIO_REMAPPED;
}
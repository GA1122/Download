static int flakey_end_io(struct dm_target *ti, struct bio *bio,
			 int error, union map_info *map_context)
{
	struct flakey_c *fc = ti->private;
	unsigned bio_submitted_while_down = map_context->ll;

	 
	if (!error && bio_submitted_while_down &&
	    (bio_data_dir(bio) == READ) && (fc->corrupt_bio_rw == READ) &&
	    all_corrupt_bio_flags_match(bio, fc))
		corrupt_bio_data(bio, fc);

	return error;
}

int main(int argc, char**argv)
{
	blkid_cache cache = NULL;
	int ret;

	blkid_init_debug(BLKID_DEBUG_ALL);
	if (argc > 2) {
		fprintf(stderr, "Usage: %s [filename]\n"
			"Test parsing of the cache (filename)\n", argv[0]);
		exit(1);
	}
	if ((ret = blkid_get_cache(&cache, argv[1])) < 0)
		fprintf(stderr, "error %d reading cache file %s\n", ret,
			argv[1] ? argv[1] : blkid_get_cache_filename(NULL));

	blkid_put_cache(cache);

	return ret;
}

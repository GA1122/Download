static int htab_create(struct libusb_context *ctx, unsigned long nel)
{
	if (htab_table != NULL) {
		usbi_err(ctx, "hash table already allocated");
	}

	usbi_mutex_init(&htab_write_mutex, NULL);

	nel |= 1;
	while(!isprime(nel))
		nel += 2;

	htab_size = nel;
	usbi_dbg("using %d entries hash table", nel);
	htab_filled = 0;

	htab_table = (htab_entry*) calloc(htab_size + 1, sizeof(htab_entry));
	if (htab_table == NULL) {
		usbi_err(ctx, "could not allocate space for hash table");
		return 0;
	}

	return 1;
}

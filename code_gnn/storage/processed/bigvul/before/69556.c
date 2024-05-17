static struct trusted_key_options *trusted_options_alloc(void)
{
	struct trusted_key_options *options;

	options = kzalloc(sizeof *options, GFP_KERNEL);
	if (options) {
		 
		options->keytype = SRK_keytype;
		options->keyhandle = SRKHANDLE;
	}
	return options;
}
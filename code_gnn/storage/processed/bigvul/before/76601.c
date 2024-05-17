static int fsck_blob(struct blob *blob, const char *buf,
		     unsigned long size, struct fsck_options *options)
{
	struct fsck_gitmodules_data data;

	if (!oidset_contains(&gitmodules_found, &blob->object.oid))
		return 0;
	oidset_insert(&gitmodules_done, &blob->object.oid);

	if (!buf) {
		 
		return report(options, &blob->object,
			      FSCK_MSG_GITMODULES_PARSE,
			      ".gitmodules too large to parse");
	}

	data.obj = &blob->object;
	data.options = options;
	data.ret = 0;
	if (git_config_from_mem(fsck_gitmodules_fn, CONFIG_ORIGIN_BLOB,
				".gitmodules", buf, size, &data))
		data.ret |= report(options, &blob->object,
				   FSCK_MSG_GITMODULES_PARSE,
				   "could not parse gitmodules blob");

	return data.ret;
}

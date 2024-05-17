sa_parse_legacy_options(sa_group_t group, char *options, char *proto)
{
	sa_fstype_t *fstype;

#ifdef DEBUG
	fprintf(stderr, "sa_parse_legacy_options: options=%s, proto=%s\n",
		options, proto);
#endif

	fstype = fstypes;
	while (fstype != NULL) {
		if (strcmp(fstype->name, proto) != 0) {
			fstype = fstype->next;
			continue;
		}

		return (fstype->ops->validate_shareopts(options));
	}

	return (SA_INVALID_PROTOCOL);
}

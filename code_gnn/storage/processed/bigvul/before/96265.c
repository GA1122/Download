 register_fstype(const char *name, const sa_share_ops_t *ops)
 {
	sa_fstype_t *fstype;

	fstype = calloc(sizeof (sa_fstype_t), 1);

	if (fstype == NULL)
		return (NULL);

	fstype->name = name;
	fstype->ops = ops;
	fstype->fsinfo_index = fstypes_count;

	fstypes_count++;

	fstype->next = fstypes;
	fstypes = fstype;

	return (fstype);
}
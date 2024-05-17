xfs_attr_get_int(
	struct xfs_inode	*ip,
	struct xfs_name		*name,
	unsigned char		*value,
	int			*valuelenp,
	int			flags)
{
	xfs_da_args_t   args;
	int             error;

	if (!xfs_inode_hasattr(ip))
		return ENOATTR;

	 
	memset((char *)&args, 0, sizeof(args));
	args.name = name->name;
	args.namelen = name->len;
	args.value = value;
	args.valuelen = *valuelenp;
	args.flags = flags;
	args.hashval = xfs_da_hashname(args.name, args.namelen);
	args.dp = ip;
	args.whichfork = XFS_ATTR_FORK;

	 
	if (ip->i_d.di_aformat == XFS_DINODE_FMT_LOCAL) {
		error = xfs_attr_shortform_getvalue(&args);
	} else if (xfs_bmap_one_block(ip, XFS_ATTR_FORK)) {
		error = xfs_attr_leaf_get(&args);
	} else {
		error = xfs_attr_node_get(&args);
	}

	 
	*valuelenp = args.valuelen;

	if (error == EEXIST)
		error = 0;
	return(error);
}

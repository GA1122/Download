xfs_attr_name_to_xname(
	struct xfs_name	*xname,
	const unsigned char *aname)
{
	if (!aname)
		return EINVAL;
	xname->name = aname;
	xname->len = strlen((char *)aname);
	if (xname->len >= MAXNAMELEN)
		return EFAULT;		 

	return 0;
}
xfs_attr_shortform_verify(
	struct xfs_inode		*ip)
{
	struct xfs_attr_shortform	*sfp;
	struct xfs_attr_sf_entry	*sfep;
	struct xfs_attr_sf_entry	*next_sfep;
	char				*endp;
	struct xfs_ifork		*ifp;
	int				i;
	int				size;

	ASSERT(ip->i_d.di_aformat == XFS_DINODE_FMT_LOCAL);
	ifp = XFS_IFORK_PTR(ip, XFS_ATTR_FORK);
	sfp = (struct xfs_attr_shortform *)ifp->if_u1.if_data;
	size = ifp->if_bytes;

	 
	if (size < sizeof(struct xfs_attr_sf_hdr))
		return __this_address;

	endp = (char *)sfp + size;

	 
	sfep = &sfp->list[0];
	for (i = 0; i < sfp->hdr.count; i++) {
		 
		if (((char *)sfep + sizeof(*sfep)) >= endp)
			return __this_address;

		 
		if (sfep->namelen == 0)
			return __this_address;

		 
		next_sfep = XFS_ATTR_SF_NEXTENTRY(sfep);
		if ((char *)next_sfep > endp)
			return __this_address;

		 
		if (sfep->flags & ~XFS_ATTR_NSP_ONDISK_MASK)
			return __this_address;

		 
		if (hweight8(sfep->flags & XFS_ATTR_NSP_ONDISK_MASK) > 1)
			return __this_address;

		sfep = next_sfep;
	}
	if ((void *)sfep != (void *)endp)
		return __this_address;

	return NULL;
}

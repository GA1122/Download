xfs_attr3_leaf_moveents(
	struct xfs_da_args		*args,
	struct xfs_attr_leafblock	*leaf_s,
	struct xfs_attr3_icleaf_hdr	*ichdr_s,
	int				start_s,
	struct xfs_attr_leafblock	*leaf_d,
	struct xfs_attr3_icleaf_hdr	*ichdr_d,
	int				start_d,
	int				count)
{
	struct xfs_attr_leaf_entry	*entry_s;
	struct xfs_attr_leaf_entry	*entry_d;
	int				desti;
	int				tmp;
	int				i;

	 
	if (count == 0)
		return;

	 
	ASSERT(ichdr_s->magic == XFS_ATTR_LEAF_MAGIC ||
	       ichdr_s->magic == XFS_ATTR3_LEAF_MAGIC);
	ASSERT(ichdr_s->magic == ichdr_d->magic);
	ASSERT(ichdr_s->count > 0 && ichdr_s->count < args->geo->blksize / 8);
	ASSERT(ichdr_s->firstused >= (ichdr_s->count * sizeof(*entry_s))
					+ xfs_attr3_leaf_hdr_size(leaf_s));
	ASSERT(ichdr_d->count < args->geo->blksize / 8);
	ASSERT(ichdr_d->firstused >= (ichdr_d->count * sizeof(*entry_d))
					+ xfs_attr3_leaf_hdr_size(leaf_d));

	ASSERT(start_s < ichdr_s->count);
	ASSERT(start_d <= ichdr_d->count);
	ASSERT(count <= ichdr_s->count);


	 
	if (start_d < ichdr_d->count) {
		tmp  = ichdr_d->count - start_d;
		tmp *= sizeof(xfs_attr_leaf_entry_t);
		entry_s = &xfs_attr3_leaf_entryp(leaf_d)[start_d];
		entry_d = &xfs_attr3_leaf_entryp(leaf_d)[start_d + count];
		memmove(entry_d, entry_s, tmp);
	}

	 
	entry_s = &xfs_attr3_leaf_entryp(leaf_s)[start_s];
	entry_d = &xfs_attr3_leaf_entryp(leaf_d)[start_d];
	desti = start_d;
	for (i = 0; i < count; entry_s++, entry_d++, desti++, i++) {
		ASSERT(be16_to_cpu(entry_s->nameidx) >= ichdr_s->firstused);
		tmp = xfs_attr_leaf_entsize(leaf_s, start_s + i);
#ifdef GROT
		 
		if (entry_s->flags & XFS_ATTR_INCOMPLETE) {  
			memset(xfs_attr3_leaf_name(leaf_s, start_s + i), 0, tmp);
			ichdr_s->usedbytes -= tmp;
			ichdr_s->count -= 1;
			entry_d--;	 
			desti--;
			if ((start_s + i) < offset)
				result++;	 
		} else {
#endif  
			ichdr_d->firstused -= tmp;
			 
			entry_d->hashval = entry_s->hashval;
			entry_d->nameidx = cpu_to_be16(ichdr_d->firstused);
			entry_d->flags = entry_s->flags;
			ASSERT(be16_to_cpu(entry_d->nameidx) + tmp
							<= args->geo->blksize);
			memmove(xfs_attr3_leaf_name(leaf_d, desti),
				xfs_attr3_leaf_name(leaf_s, start_s + i), tmp);
			ASSERT(be16_to_cpu(entry_s->nameidx) + tmp
							<= args->geo->blksize);
			memset(xfs_attr3_leaf_name(leaf_s, start_s + i), 0, tmp);
			ichdr_s->usedbytes -= tmp;
			ichdr_d->usedbytes += tmp;
			ichdr_s->count -= 1;
			ichdr_d->count += 1;
			tmp = ichdr_d->count * sizeof(xfs_attr_leaf_entry_t)
					+ xfs_attr3_leaf_hdr_size(leaf_d);
			ASSERT(ichdr_d->firstused >= tmp);
#ifdef GROT
		}
#endif  
	}

	 
	if (start_s == ichdr_s->count) {
		tmp = count * sizeof(xfs_attr_leaf_entry_t);
		entry_s = &xfs_attr3_leaf_entryp(leaf_s)[start_s];
		ASSERT(((char *)entry_s + tmp) <=
		       ((char *)leaf_s + args->geo->blksize));
		memset(entry_s, 0, tmp);
	} else {
		 
		tmp  = (ichdr_s->count - count) * sizeof(xfs_attr_leaf_entry_t);
		entry_s = &xfs_attr3_leaf_entryp(leaf_s)[start_s + count];
		entry_d = &xfs_attr3_leaf_entryp(leaf_s)[start_s];
		memmove(entry_d, entry_s, tmp);

		tmp = count * sizeof(xfs_attr_leaf_entry_t);
		entry_s = &xfs_attr3_leaf_entryp(leaf_s)[ichdr_s->count];
		ASSERT(((char *)entry_s + tmp) <=
		       ((char *)leaf_s + args->geo->blksize));
		memset(entry_s, 0, tmp);
	}

	 
	ichdr_d->freemap[0].base = xfs_attr3_leaf_hdr_size(leaf_d);
	ichdr_d->freemap[0].base += ichdr_d->count * sizeof(xfs_attr_leaf_entry_t);
	ichdr_d->freemap[0].size = ichdr_d->firstused - ichdr_d->freemap[0].base;
	ichdr_d->freemap[1].base = 0;
	ichdr_d->freemap[2].base = 0;
	ichdr_d->freemap[1].size = 0;
	ichdr_d->freemap[2].size = 0;
	ichdr_s->holes = 1;	 
}

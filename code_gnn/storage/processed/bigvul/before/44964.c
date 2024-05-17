xfs_attr_put_listent(
	xfs_attr_list_context_t *context,
	int		flags,
	unsigned char	*name,
	int		namelen,
	int		valuelen,
	unsigned char	*value)
{
	struct attrlist *alist = (struct attrlist *)context->alist;
	attrlist_ent_t *aep;
	int arraytop;

	ASSERT(!(context->flags & ATTR_KERNOVAL));
	ASSERT(context->count >= 0);
	ASSERT(context->count < (ATTR_MAX_VALUELEN/8));
	ASSERT(context->firstu >= sizeof(*alist));
	ASSERT(context->firstu <= context->bufsize);

	 
	if (((context->flags & ATTR_SECURE) == 0) !=
	    ((flags & XFS_ATTR_SECURE) == 0))
		return 0;
	if (((context->flags & ATTR_ROOT) == 0) !=
	    ((flags & XFS_ATTR_ROOT) == 0))
		return 0;

	arraytop = sizeof(*alist) +
			context->count * sizeof(alist->al_offset[0]);
	context->firstu -= ATTR_ENTSIZE(namelen);
	if (context->firstu < arraytop) {
		trace_xfs_attr_list_full(context);
		alist->al_more = 1;
		context->seen_enough = 1;
		return 1;
	}

	aep = (attrlist_ent_t *)&context->alist[context->firstu];
	aep->a_valuelen = valuelen;
	memcpy(aep->a_name, name, namelen);
	aep->a_name[namelen] = 0;
	alist->al_offset[context->count++] = context->firstu;
	alist->al_count = context->count;
	trace_xfs_attr_list_add(context);
	return 0;
}
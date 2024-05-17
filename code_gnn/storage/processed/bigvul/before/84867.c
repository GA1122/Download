static inline int ebt_entry_to_user(struct ebt_entry *e, const char *base,
				    char __user *ubase)
{
	int ret;
	char __user *hlp;
	const struct ebt_entry_target *t;

	if (e->bitmask == 0) {
		 
		if (copy_to_user(ubase + ((char *)e - base), e,
				 sizeof(struct ebt_entries)))
			return -EFAULT;
		return 0;
	}

	if (copy_to_user(ubase + ((char *)e - base), e, sizeof(*e)))
		return -EFAULT;

	hlp = ubase + (((char *)e + e->target_offset) - base);
	t = (struct ebt_entry_target *)(((char *)e) + e->target_offset);

	ret = EBT_MATCH_ITERATE(e, ebt_match_to_user, base, ubase);
	if (ret != 0)
		return ret;
	ret = EBT_WATCHER_ITERATE(e, ebt_watcher_to_user, base, ubase);
	if (ret != 0)
		return ret;
	ret = ebt_obj_to_user(hlp, t->u.target->name, t->data, sizeof(*t),
			      t->u.target->usersize, t->target_size);
	if (ret != 0)
		return ret;

	return 0;
}
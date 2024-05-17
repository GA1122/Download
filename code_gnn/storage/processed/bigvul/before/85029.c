static int get_info(struct net *net, void __user *user,
		    const int *len, int compat)
{
	char name[XT_TABLE_MAXNAMELEN];
	struct xt_table *t;
	int ret;

	if (*len != sizeof(struct ip6t_getinfo))
		return -EINVAL;

	if (copy_from_user(name, user, sizeof(name)) != 0)
		return -EFAULT;

	name[XT_TABLE_MAXNAMELEN-1] = '\0';
#ifdef CONFIG_COMPAT
	if (compat)
		xt_compat_lock(AF_INET6);
#endif
	t = xt_request_find_table_lock(net, AF_INET6, name);
	if (!IS_ERR(t)) {
		struct ip6t_getinfo info;
		const struct xt_table_info *private = t->private;
#ifdef CONFIG_COMPAT
		struct xt_table_info tmp;

		if (compat) {
			ret = compat_table_info(private, &tmp);
			xt_compat_flush_offsets(AF_INET6);
			private = &tmp;
		}
#endif
		memset(&info, 0, sizeof(info));
		info.valid_hooks = t->valid_hooks;
		memcpy(info.hook_entry, private->hook_entry,
		       sizeof(info.hook_entry));
		memcpy(info.underflow, private->underflow,
		       sizeof(info.underflow));
		info.num_entries = private->number;
		info.size = private->size;
		strcpy(info.name, name);

		if (copy_to_user(user, &info, *len) != 0)
			ret = -EFAULT;
		else
			ret = 0;

		xt_table_unlock(t);
		module_put(t->me);
	} else
		ret = PTR_ERR(t);
#ifdef CONFIG_COMPAT
	if (compat)
		xt_compat_unlock(AF_INET6);
#endif
	return ret;
}
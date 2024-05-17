static int compat_do_ebt_get_ctl(struct sock *sk, int cmd,
		void __user *user, int *len)
{
	int ret;
	struct compat_ebt_replace tmp;
	struct ebt_table *t;
	struct net *net = sock_net(sk);

	if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
		return -EPERM;

	 
	if ((cmd == EBT_SO_GET_INFO ||
	     cmd == EBT_SO_GET_INIT_INFO) && *len != sizeof(tmp))
			return do_ebt_get_ctl(sk, cmd, user, len);

	if (copy_from_user(&tmp, user, sizeof(tmp)))
		return -EFAULT;

	tmp.name[sizeof(tmp.name) - 1] = '\0';

	t = find_table_lock(net, tmp.name, &ret, &ebt_mutex);
	if (!t)
		return ret;

	xt_compat_lock(NFPROTO_BRIDGE);
	switch (cmd) {
	case EBT_SO_GET_INFO:
		tmp.nentries = t->private->nentries;
		ret = compat_table_info(t->private, &tmp);
		if (ret)
			goto out;
		tmp.valid_hooks = t->valid_hooks;

		if (copy_to_user(user, &tmp, *len) != 0) {
			ret = -EFAULT;
			break;
		}
		ret = 0;
		break;
	case EBT_SO_GET_INIT_INFO:
		tmp.nentries = t->table->nentries;
		tmp.entries_size = t->table->entries_size;
		tmp.valid_hooks = t->table->valid_hooks;

		if (copy_to_user(user, &tmp, *len) != 0) {
			ret = -EFAULT;
			break;
		}
		ret = 0;
		break;
	case EBT_SO_GET_ENTRIES:
	case EBT_SO_GET_INIT_ENTRIES:
		 
		if (copy_everything_to_user(t, user, len, cmd) == 0)
			ret = 0;
		else
			ret = compat_copy_everything_to_user(t, user, len, cmd);
		break;
	default:
		ret = -EINVAL;
	}
 out:
	xt_compat_flush_offsets(NFPROTO_BRIDGE);
	xt_compat_unlock(NFPROTO_BRIDGE);
	mutex_unlock(&ebt_mutex);
	return ret;
}
static int do_ebt_get_ctl(struct sock *sk, int cmd, void __user *user, int *len)
{
	int ret;
	struct ebt_replace tmp;
	struct ebt_table *t;
	struct net *net = sock_net(sk);

	if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
		return -EPERM;

	if (copy_from_user(&tmp, user, sizeof(tmp)))
		return -EFAULT;

	tmp.name[sizeof(tmp.name) - 1] = '\0';

	t = find_table_lock(net, tmp.name, &ret, &ebt_mutex);
	if (!t)
		return ret;

	switch (cmd) {
	case EBT_SO_GET_INFO:
	case EBT_SO_GET_INIT_INFO:
		if (*len != sizeof(struct ebt_replace)) {
			ret = -EINVAL;
			mutex_unlock(&ebt_mutex);
			break;
		}
		if (cmd == EBT_SO_GET_INFO) {
			tmp.nentries = t->private->nentries;
			tmp.entries_size = t->private->entries_size;
			tmp.valid_hooks = t->valid_hooks;
		} else {
			tmp.nentries = t->table->nentries;
			tmp.entries_size = t->table->entries_size;
			tmp.valid_hooks = t->table->valid_hooks;
		}
		mutex_unlock(&ebt_mutex);
		if (copy_to_user(user, &tmp, *len) != 0) {
			BUGPRINT("c2u Didn't work\n");
			ret = -EFAULT;
			break;
		}
		ret = 0;
		break;

	case EBT_SO_GET_ENTRIES:
	case EBT_SO_GET_INIT_ENTRIES:
		ret = copy_everything_to_user(t, user, len, cmd);
		mutex_unlock(&ebt_mutex);
		break;

	default:
		mutex_unlock(&ebt_mutex);
		ret = -EINVAL;
	}

	return ret;
}
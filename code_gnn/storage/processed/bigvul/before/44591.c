int lxc_clear_nic(struct lxc_conf *c, const char *key)
{
	char *p1;
	int ret, idx, i;
	struct lxc_list *it;
	struct lxc_netdev *netdev;

	p1 = strchr(key, '.');
	if (!p1 || *(p1+1) == '\0')
		p1 = NULL;

	ret = sscanf(key, "%d", &idx);
	if (ret != 1) return -1;
	if (idx < 0)
		return -1;

	i = 0;
	lxc_list_for_each(it, &c->network) {
		if (i == idx)
			break;
		i++;
	}
	if (i < idx)   
		return -1;

	if (!it || !it->elem)
		return -1;

	netdev = it->elem;

	if (!p1) {
		lxc_remove_nic(it);
	} else if (strcmp(p1, ".ipv4") == 0) {
		struct lxc_list *it2,*next;
		lxc_list_for_each_safe(it2, &netdev->ipv4, next) {
			lxc_list_del(it2);
			free(it2->elem);
			free(it2);
		}
	} else if (strcmp(p1, ".ipv6") == 0) {
		struct lxc_list *it2,*next;
		lxc_list_for_each_safe(it2, &netdev->ipv6, next) {
			lxc_list_del(it2);
			free(it2->elem);
			free(it2);
		}
	}
		else return -1;

	return 0;
}
static int __dev_alloc_name(struct net *net, const char *name, char *buf)
{
	int i = 0;
	const char *p;
	const int max_netdevices = 8*PAGE_SIZE;
	unsigned long *inuse;
	struct net_device *d;

	p = strnchr(name, IFNAMSIZ-1, '%');
	if (p) {
		 
		if (p[1] != 'd' || strchr(p + 2, '%'))
			return -EINVAL;

		 
		inuse = (unsigned long *) get_zeroed_page(GFP_ATOMIC);
		if (!inuse)
			return -ENOMEM;

		for_each_netdev(net, d) {
			if (!sscanf(d->name, name, &i))
				continue;
			if (i < 0 || i >= max_netdevices)
				continue;

			 
			snprintf(buf, IFNAMSIZ, name, i);
			if (!strncmp(buf, d->name, IFNAMSIZ))
				set_bit(i, inuse);
		}

		i = find_first_zero_bit(inuse, max_netdevices);
		free_page((unsigned long) inuse);
	}

	if (buf != name)
		snprintf(buf, IFNAMSIZ, name, i);
	if (!__dev_get_by_name(net, buf))
		return i;

	 
	return -ENFILE;
}
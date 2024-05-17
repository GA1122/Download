static int update_filter(struct tap_filter *filter, void __user *arg)
{
	struct { u8 u[ETH_ALEN]; } *addr;
	struct tun_filter uf;
	int err, alen, n, nexact;

	if (copy_from_user(&uf, arg, sizeof(uf)))
		return -EFAULT;

	if (!uf.count) {
		 
		filter->count = 0;
		return 0;
	}

	alen = ETH_ALEN * uf.count;
	addr = memdup_user(arg + sizeof(uf), alen);
	if (IS_ERR(addr))
		return PTR_ERR(addr);

	 
	filter->count = 0;
	wmb();

	 
	for (n = 0; n < uf.count && n < FLT_EXACT_COUNT; n++)
		memcpy(filter->addr[n], addr[n].u, ETH_ALEN);

	nexact = n;

	 
	memset(filter->mask, 0, sizeof(filter->mask));
	for (; n < uf.count; n++) {
		if (!is_multicast_ether_addr(addr[n].u)) {
			err = 0;  
			goto free_addr;
		}
		addr_hash_set(filter->mask, addr[n].u);
	}

	 
	if ((uf.flags & TUN_FLT_ALLMULTI))
		memset(filter->mask, ~0, sizeof(filter->mask));

	 
	wmb();
	filter->count = nexact;

	 
	err = nexact;
free_addr:
	kfree(addr);
	return err;
}

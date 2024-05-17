static void manage_tempaddrs(struct inet6_dev *idev,
			     struct inet6_ifaddr *ifp,
			     __u32 valid_lft, __u32 prefered_lft,
			     bool create, unsigned long now)
{
	u32 flags;
	struct inet6_ifaddr *ift;

	read_lock_bh(&idev->lock);
	 
	list_for_each_entry(ift, &idev->tempaddr_list, tmp_list) {
		int age, max_valid, max_prefered;

		if (ifp != ift->ifpub)
			continue;

		 
		age = (now - ift->cstamp) / HZ;
		max_valid = idev->cnf.temp_valid_lft - age;
		if (max_valid < 0)
			max_valid = 0;

		max_prefered = idev->cnf.temp_prefered_lft -
			       idev->cnf.max_desync_factor - age;
		if (max_prefered < 0)
			max_prefered = 0;

		if (valid_lft > max_valid)
			valid_lft = max_valid;

		if (prefered_lft > max_prefered)
			prefered_lft = max_prefered;

		spin_lock(&ift->lock);
		flags = ift->flags;
		ift->valid_lft = valid_lft;
		ift->prefered_lft = prefered_lft;
		ift->tstamp = now;
		if (prefered_lft > 0)
			ift->flags &= ~IFA_F_DEPRECATED;

		spin_unlock(&ift->lock);
		if (!(flags&IFA_F_TENTATIVE))
			ipv6_ifa_notify(0, ift);
	}

	if ((create || list_empty(&idev->tempaddr_list)) &&
	    idev->cnf.use_tempaddr > 0) {
		 
		read_unlock_bh(&idev->lock);
		ipv6_create_tempaddr(ifp, NULL);
	} else {
		read_unlock_bh(&idev->lock);
	}
}

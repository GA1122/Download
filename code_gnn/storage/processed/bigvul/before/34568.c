static long macvtap_ioctl(struct file *file, unsigned int cmd,
			  unsigned long arg)
{
	struct macvtap_queue *q = file->private_data;
	struct macvlan_dev *vlan;
	void __user *argp = (void __user *)arg;
	struct ifreq __user *ifr = argp;
	unsigned int __user *up = argp;
	unsigned int u;
	int __user *sp = argp;
	int s;
	int ret;

	switch (cmd) {
	case TUNSETIFF:
		 
		if (get_user(u, &ifr->ifr_flags))
			return -EFAULT;

		ret = 0;
		if ((u & ~IFF_VNET_HDR) != (IFF_NO_PI | IFF_TAP))
			ret = -EINVAL;
		else
			q->flags = u;

		return ret;

	case TUNGETIFF:
		rcu_read_lock_bh();
		vlan = rcu_dereference_bh(q->vlan);
		if (vlan)
			dev_hold(vlan->dev);
		rcu_read_unlock_bh();

		if (!vlan)
			return -ENOLINK;

		ret = 0;
		if (copy_to_user(&ifr->ifr_name, vlan->dev->name, IFNAMSIZ) ||
		    put_user(q->flags, &ifr->ifr_flags))
			ret = -EFAULT;
		dev_put(vlan->dev);
		return ret;

	case TUNGETFEATURES:
		if (put_user(IFF_TAP | IFF_NO_PI | IFF_VNET_HDR, up))
			return -EFAULT;
		return 0;

	case TUNSETSNDBUF:
		if (get_user(u, up))
			return -EFAULT;

		q->sk.sk_sndbuf = u;
		return 0;

	case TUNGETVNETHDRSZ:
		s = q->vnet_hdr_sz;
		if (put_user(s, sp))
			return -EFAULT;
		return 0;

	case TUNSETVNETHDRSZ:
		if (get_user(s, sp))
			return -EFAULT;
		if (s < (int)sizeof(struct virtio_net_hdr))
			return -EINVAL;

		q->vnet_hdr_sz = s;
		return 0;

	case TUNSETOFFLOAD:
		 
		if (arg & ~(TUN_F_CSUM | TUN_F_TSO4 | TUN_F_TSO6 |
			    TUN_F_TSO_ECN | TUN_F_UFO))
			return -EINVAL;

		 
		if (!(q->flags & IFF_VNET_HDR))
			return  -EINVAL;
		return 0;

	default:
		return -EINVAL;
	}
}
static int __sctp_setsockopt_connectx(struct sock *sk,
				      struct sockaddr __user *addrs,
				      int addrs_size,
				      sctp_assoc_t *assoc_id)
{
	int err = 0;
	struct sockaddr *kaddrs;

	pr_debug("%s: sk:%p addrs:%p addrs_size:%d\n",
		 __func__, sk, addrs, addrs_size);

	if (unlikely(addrs_size <= 0))
		return -EINVAL;

	 
	if (unlikely(!access_ok(VERIFY_READ, addrs, addrs_size)))
		return -EFAULT;

	 
	kaddrs = kmalloc(addrs_size, GFP_KERNEL);
	if (unlikely(!kaddrs))
		return -ENOMEM;

	if (__copy_from_user(kaddrs, addrs, addrs_size)) {
		err = -EFAULT;
	} else {
		err = __sctp_connect(sk, kaddrs, addrs_size, assoc_id);
	}

	kfree(kaddrs);

	return err;
}
static int sctp_getsockopt_connectx3(struct sock *sk, int len,
				     char __user *optval,
				     int __user *optlen)
{
	struct sctp_getaddrs_old param;
	sctp_assoc_t assoc_id = 0;
	int err = 0;

#ifdef CONFIG_COMPAT
	if (is_compat_task()) {
		struct compat_sctp_getaddrs_old param32;

		if (len < sizeof(param32))
			return -EINVAL;
		if (copy_from_user(&param32, optval, sizeof(param32)))
			return -EFAULT;

		param.assoc_id = param32.assoc_id;
		param.addr_num = param32.addr_num;
		param.addrs = compat_ptr(param32.addrs);
	} else
#endif
	{
		if (len < sizeof(param))
			return -EINVAL;
		if (copy_from_user(&param, optval, sizeof(param)))
			return -EFAULT;
	}

	err = __sctp_setsockopt_connectx(sk, (struct sockaddr __user *)
					 param.addrs, param.addr_num,
					 &assoc_id);
	if (err == 0 || err == -EINPROGRESS) {
		if (copy_to_user(optval, &assoc_id, sizeof(assoc_id)))
			return -EFAULT;
		if (put_user(sizeof(assoc_id), optlen))
			return -EFAULT;
	}

	return err;
}
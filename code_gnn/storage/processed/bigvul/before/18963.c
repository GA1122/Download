static int raw_probe_proto_opt(struct flowi4 *fl4, struct msghdr *msg)
{
	struct iovec *iov;
	u8 __user *type = NULL;
	u8 __user *code = NULL;
	int probed = 0;
	unsigned int i;

	if (!msg->msg_iov)
		return 0;

	for (i = 0; i < msg->msg_iovlen; i++) {
		iov = &msg->msg_iov[i];
		if (!iov)
			continue;

		switch (fl4->flowi4_proto) {
		case IPPROTO_ICMP:
			 
			if (iov->iov_base && iov->iov_len < 1)
				break;

			if (!type) {
				type = iov->iov_base;
				 
				if (iov->iov_len > 1)
					code = type + 1;
			} else if (!code)
				code = iov->iov_base;

			if (type && code) {
				if (get_user(fl4->fl4_icmp_type, type) ||
				    get_user(fl4->fl4_icmp_code, code))
					return -EFAULT;
				probed = 1;
			}
			break;
		default:
			probed = 1;
			break;
		}
		if (probed)
			break;
	}
	return 0;
}
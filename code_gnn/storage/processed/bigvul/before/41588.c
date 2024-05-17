static int irda_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg)
{
	struct sock *sk = sock->sk;
	int err;

	pr_debug("%s(), cmd=%#x\n", __func__, cmd);

	err = -EINVAL;
	switch (cmd) {
	case TIOCOUTQ: {
		long amount;

		amount = sk->sk_sndbuf - sk_wmem_alloc_get(sk);
		if (amount < 0)
			amount = 0;
		err = put_user(amount, (unsigned int __user *)arg);
		break;
	}

	case TIOCINQ: {
		struct sk_buff *skb;
		long amount = 0L;
		 
		if ((skb = skb_peek(&sk->sk_receive_queue)) != NULL)
			amount = skb->len;
		err = put_user(amount, (unsigned int __user *)arg);
		break;
	}

	case SIOCGSTAMP:
		if (sk != NULL)
			err = sock_get_timestamp(sk, (struct timeval __user *)arg);
		break;

	case SIOCGIFADDR:
	case SIOCSIFADDR:
	case SIOCGIFDSTADDR:
	case SIOCSIFDSTADDR:
	case SIOCGIFBRDADDR:
	case SIOCSIFBRDADDR:
	case SIOCGIFNETMASK:
	case SIOCSIFNETMASK:
	case SIOCGIFMETRIC:
	case SIOCSIFMETRIC:
		break;
	default:
		pr_debug("%s(), doing device ioctl!\n", __func__);
		err = -ENOIOCTLCMD;
	}

	return err;
}
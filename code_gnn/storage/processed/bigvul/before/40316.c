static int atalk_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg)
{
	int rc = -ENOIOCTLCMD;
	struct sock *sk = sock->sk;
	void __user *argp = (void __user *)arg;

	switch (cmd) {
		 
		case TIOCOUTQ: {
			long amount = sk->sk_sndbuf - sk_wmem_alloc_get(sk);

			if (amount < 0)
				amount = 0;
			rc = put_user(amount, (int __user *)argp);
			break;
		}
		case TIOCINQ: {
			 
			struct sk_buff *skb = skb_peek(&sk->sk_receive_queue);
			long amount = 0;

			if (skb)
				amount = skb->len - sizeof(struct ddpehdr);
			rc = put_user(amount, (int __user *)argp);
			break;
		}
		case SIOCGSTAMP:
			rc = sock_get_timestamp(sk, argp);
			break;
		case SIOCGSTAMPNS:
			rc = sock_get_timestampns(sk, argp);
			break;
		 
		case SIOCADDRT:
		case SIOCDELRT:
			rc = -EPERM;
			if (capable(CAP_NET_ADMIN))
				rc = atrtr_ioctl(cmd, argp);
			break;
		 
		case SIOCGIFADDR:
		case SIOCSIFADDR:
		case SIOCGIFBRDADDR:
		case SIOCATALKDIFADDR:
		case SIOCDIFADDR:
		case SIOCSARP:		 
		case SIOCDARP:		 
			rtnl_lock();
			rc = atif_ioctl(cmd, argp);
			rtnl_unlock();
			break;
	}

	return rc;
}

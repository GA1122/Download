static int sctp_ioctl(struct sock *sk, int cmd, unsigned long arg)
{
	int rc = -ENOTCONN;

	lock_sock(sk);

	 
	if (sctp_style(sk, TCP) && sctp_sstate(sk, LISTENING))
		goto out;

	switch (cmd) {
	case SIOCINQ: {
		struct sk_buff *skb;
		unsigned int amount = 0;

		skb = skb_peek(&sk->sk_receive_queue);
		if (skb != NULL) {
			 
			amount = skb->len;
		}
		rc = put_user(amount, (int __user *)arg);
		break;
	}
	default:
		rc = -ENOIOCTLCMD;
		break;
	}
out:
	release_sock(sk);
	return rc;
}

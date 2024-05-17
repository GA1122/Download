static int l2cap_sock_recv_cb(struct l2cap_chan *chan, struct sk_buff *skb)
{
	int err;
	struct sock *sk = chan->data;
	struct l2cap_pinfo *pi = l2cap_pi(sk);

	lock_sock(sk);

	if (pi->rx_busy_skb) {
		err = -ENOMEM;
		goto done;
	}

	err = sock_queue_rcv_skb(sk, skb);

	 
	if (err < 0 && pi->chan->mode == L2CAP_MODE_ERTM) {
		pi->rx_busy_skb = skb;
		l2cap_chan_busy(pi->chan, 1);
		err = 0;
	}

done:
	release_sock(sk);

	return err;
}

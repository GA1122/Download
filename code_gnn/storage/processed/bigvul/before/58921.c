static void l2cap_chan_ready(struct sock *sk)
{
	struct sock *parent = bt_sk(sk)->parent;

	BT_DBG("sk %p, parent %p", sk, parent);

	l2cap_pi(sk)->conf_state = 0;
	l2cap_sock_clear_timer(sk);

	if (!parent) {
		 
		sk->sk_state = BT_CONNECTED;
		sk->sk_state_change(sk);
	} else {
		 
		parent->sk_data_ready(parent, 0);
	}
}
static void l2cap_sock_cleanup_listen(struct sock *parent)
{
	struct sock *sk;

	BT_DBG("parent %p", parent);

	 
	while ((sk = bt_accept_dequeue(parent, NULL)))
		l2cap_sock_close(sk);

	parent->sk_state = BT_CLOSED;
	sock_set_flag(parent, SOCK_ZAPPED);
}
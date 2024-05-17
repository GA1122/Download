static void l2cap_sock_kill(struct sock *sk)
{
	if (!sock_flag(sk, SOCK_ZAPPED) || sk->sk_socket)
		return;

	BT_DBG("sk %p state %d", sk, sk->sk_state);

	 
	bt_sock_unlink(&l2cap_sk_list, sk);
	sock_set_flag(sk, SOCK_DEAD);
	sock_put(sk);
}
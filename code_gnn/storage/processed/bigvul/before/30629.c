void iucv_sock_unlink(struct iucv_sock_list *l, struct sock *sk)
{
	write_lock_bh(&l->lock);
	sk_del_node_init(sk);
	write_unlock_bh(&l->lock);
}
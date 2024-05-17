static void inet_child_forget(struct sock *sk, struct request_sock *req,
			      struct sock *child)
{
	sk->sk_prot->disconnect(child, O_NONBLOCK);

	sock_orphan(child);

	percpu_counter_inc(sk->sk_prot->orphan_count);

	if (sk->sk_protocol == IPPROTO_TCP && tcp_rsk(req)->tfo_listener) {
		BUG_ON(tcp_sk(child)->fastopen_rsk != req);
		BUG_ON(sk != req->rsk_listener);

		 
		tcp_sk(child)->fastopen_rsk = NULL;
	}
	inet_csk_destroy_sock(child);
	reqsk_put(req);
}
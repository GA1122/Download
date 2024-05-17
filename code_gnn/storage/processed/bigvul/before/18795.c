ssize_t inet_sendpage(struct socket *sock, struct page *page, int offset,
		      size_t size, int flags)
{
	struct sock *sk = sock->sk;

	sock_rps_record_flow(sk);

	 
	if (!inet_sk(sk)->inet_num && !sk->sk_prot->no_autobind &&
	    inet_autobind(sk))
		return -EAGAIN;

	if (sk->sk_prot->sendpage)
		return sk->sk_prot->sendpage(sk, page, offset, size, flags);
	return sock_no_sendpage(sock, page, offset, size, flags);
}
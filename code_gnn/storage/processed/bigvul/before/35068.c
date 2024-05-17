static inline int sctp_wspace(struct sctp_association *asoc)
{
	struct sock *sk = asoc->base.sk;
	int amt = 0;

	if (asoc->ep->sndbuf_policy) {
		 
		amt = sk->sk_sndbuf - asoc->sndbuf_used;
	} else {
		 
		amt = sk->sk_sndbuf - atomic_read(&sk->sk_wmem_alloc);
	}

	if (amt < 0)
		amt = 0;

	return amt;
}

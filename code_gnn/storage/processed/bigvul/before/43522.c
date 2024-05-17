static int sctp_connect(struct sock *sk, struct sockaddr *addr,
			int addr_len)
{
	int err = 0;
	struct sctp_af *af;

	lock_sock(sk);

	pr_debug("%s: sk:%p, sockaddr:%p, addr_len:%d\n", __func__, sk,
		 addr, addr_len);

	 
	af = sctp_get_af_specific(addr->sa_family);
	if (!af || addr_len < af->sockaddr_len) {
		err = -EINVAL;
	} else {
		 
		err = __sctp_connect(sk, addr, af->sockaddr_len, NULL);
	}

	release_sock(sk);
	return err;
}
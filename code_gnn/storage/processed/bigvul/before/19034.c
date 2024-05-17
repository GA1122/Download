			tcp_v4_md5_do_lookup(struct sock *sk, __be32 addr)
{
	struct tcp_sock *tp = tcp_sk(sk);
	int i;

	if (!tp->md5sig_info || !tp->md5sig_info->entries4)
		return NULL;
	for (i = 0; i < tp->md5sig_info->entries4; i++) {
		if (tp->md5sig_info->keys4[i].addr == addr)
			return &tp->md5sig_info->keys4[i].base;
	}
	return NULL;
}

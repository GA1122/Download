static int tcp_v6_md5_do_del(struct sock *sk, const struct in6_addr *peer)
{
	struct tcp_sock *tp = tcp_sk(sk);
	int i;

	for (i = 0; i < tp->md5sig_info->entries6; i++) {
		if (ipv6_addr_equal(&tp->md5sig_info->keys6[i].addr, peer)) {
			 
			kfree(tp->md5sig_info->keys6[i].base.key);
			tp->md5sig_info->entries6--;

			if (tp->md5sig_info->entries6 == 0) {
				kfree(tp->md5sig_info->keys6);
				tp->md5sig_info->keys6 = NULL;
				tp->md5sig_info->alloced6 = 0;
			} else {
				 
				if (tp->md5sig_info->entries6 != i)
					memmove(&tp->md5sig_info->keys6[i],
						&tp->md5sig_info->keys6[i+1],
						(tp->md5sig_info->entries6 - i)
						* sizeof (tp->md5sig_info->keys6[0]));
			}
			tcp_free_md5sig_pool();
			return 0;
		}
	}
	return -ENOENT;
}
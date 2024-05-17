int tcp_v4_md5_do_del(struct sock *sk, __be32 addr)
{
	struct tcp_sock *tp = tcp_sk(sk);
	int i;

	for (i = 0; i < tp->md5sig_info->entries4; i++) {
		if (tp->md5sig_info->keys4[i].addr == addr) {
			 
			kfree(tp->md5sig_info->keys4[i].base.key);
			tp->md5sig_info->entries4--;

			if (tp->md5sig_info->entries4 == 0) {
				kfree(tp->md5sig_info->keys4);
				tp->md5sig_info->keys4 = NULL;
				tp->md5sig_info->alloced4 = 0;
			} else if (tp->md5sig_info->entries4 != i) {
				 
				memmove(&tp->md5sig_info->keys4[i],
					&tp->md5sig_info->keys4[i+1],
					(tp->md5sig_info->entries4 - i) *
					 sizeof(struct tcp4_md5sig_key));
			}
			tcp_free_md5sig_pool();
			return 0;
		}
	}
	return -ENOENT;
}
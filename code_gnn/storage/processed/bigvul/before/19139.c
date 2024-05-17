static int tcp_v6_md5_do_add(struct sock *sk, const struct in6_addr *peer,
			     char *newkey, u8 newkeylen)
{
	 
	struct tcp_md5sig_key *key;
	struct tcp_sock *tp = tcp_sk(sk);
	struct tcp6_md5sig_key *keys;

	key = tcp_v6_md5_do_lookup(sk, peer);
	if (key) {
		 
		kfree(key->key);
		key->key = newkey;
		key->keylen = newkeylen;
	} else {
		 
		if (!tp->md5sig_info) {
			tp->md5sig_info = kzalloc(sizeof(*tp->md5sig_info), GFP_ATOMIC);
			if (!tp->md5sig_info) {
				kfree(newkey);
				return -ENOMEM;
			}
			sk_nocaps_add(sk, NETIF_F_GSO_MASK);
		}
		if (tcp_alloc_md5sig_pool(sk) == NULL) {
			kfree(newkey);
			return -ENOMEM;
		}
		if (tp->md5sig_info->alloced6 == tp->md5sig_info->entries6) {
			keys = kmalloc((sizeof (tp->md5sig_info->keys6[0]) *
				       (tp->md5sig_info->entries6 + 1)), GFP_ATOMIC);

			if (!keys) {
				tcp_free_md5sig_pool();
				kfree(newkey);
				return -ENOMEM;
			}

			if (tp->md5sig_info->entries6)
				memmove(keys, tp->md5sig_info->keys6,
					(sizeof (tp->md5sig_info->keys6[0]) *
					 tp->md5sig_info->entries6));

			kfree(tp->md5sig_info->keys6);
			tp->md5sig_info->keys6 = keys;
			tp->md5sig_info->alloced6++;
		}

		ipv6_addr_copy(&tp->md5sig_info->keys6[tp->md5sig_info->entries6].addr,
			       peer);
		tp->md5sig_info->keys6[tp->md5sig_info->entries6].base.key = newkey;
		tp->md5sig_info->keys6[tp->md5sig_info->entries6].base.keylen = newkeylen;

		tp->md5sig_info->entries6++;
	}
	return 0;
}
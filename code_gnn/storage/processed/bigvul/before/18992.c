static void *established_get_next(struct seq_file *seq, void *cur)
{
	struct sock *sk = cur;
	struct inet_timewait_sock *tw;
	struct hlist_nulls_node *node;
	struct tcp_iter_state *st = seq->private;
	struct net *net = seq_file_net(seq);

	++st->num;
	++st->offset;

	if (st->state == TCP_SEQ_STATE_TIME_WAIT) {
		tw = cur;
		tw = tw_next(tw);
get_tw:
		while (tw && (tw->tw_family != st->family || !net_eq(twsk_net(tw), net))) {
			tw = tw_next(tw);
		}
		if (tw) {
			cur = tw;
			goto out;
		}
		spin_unlock_bh(inet_ehash_lockp(&tcp_hashinfo, st->bucket));
		st->state = TCP_SEQ_STATE_ESTABLISHED;

		 
		st->offset = 0;
		while (++st->bucket <= tcp_hashinfo.ehash_mask &&
				empty_bucket(st))
			;
		if (st->bucket > tcp_hashinfo.ehash_mask)
			return NULL;

		spin_lock_bh(inet_ehash_lockp(&tcp_hashinfo, st->bucket));
		sk = sk_nulls_head(&tcp_hashinfo.ehash[st->bucket].chain);
	} else
		sk = sk_nulls_next(sk);

	sk_nulls_for_each_from(sk, node) {
		if (sk->sk_family == st->family && net_eq(sock_net(sk), net))
			goto found;
	}

	st->state = TCP_SEQ_STATE_TIME_WAIT;
	tw = tw_head(&tcp_hashinfo.ehash[st->bucket].twchain);
	goto get_tw;
found:
	cur = sk;
out:
	return cur;
}
static int dn_data_ready(struct sock *sk, struct sk_buff_head *q, int flags, int target)
{
	struct sk_buff *skb;
	int len = 0;

	if (flags & MSG_OOB)
		return !skb_queue_empty(q) ? 1 : 0;

	skb_queue_walk(q, skb) {
		struct dn_skb_cb *cb = DN_SKB_CB(skb);
		len += skb->len;

		if (cb->nsp_flags & 0x40) {
			 
			if (sk->sk_type == SOCK_SEQPACKET)
				return 1;
			 
			if (!(flags & MSG_WAITALL))
				return 1;
		}

		 
		if (len >= target)
			return 1;
	}

	return 0;
}

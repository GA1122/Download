ppp_mp_insert(struct ppp *ppp, struct sk_buff *skb)
{
	struct sk_buff *p;
	struct sk_buff_head *list = &ppp->mrq;
	u32 seq = PPP_MP_CB(skb)->sequence;

	 
	skb_queue_walk(list, p) {
		if (seq_before(seq, PPP_MP_CB(p)->sequence))
			break;
	}
	__skb_queue_before(list, p, skb);
}

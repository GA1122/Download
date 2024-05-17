static int raw_rcv_skb(struct sock * sk, struct sk_buff * skb)
{
	 

	if (ip_queue_rcv_skb(sk, skb) < 0) {
		kfree_skb(skb);
		return NET_RX_DROP;
	}

	return NET_RX_SUCCESS;
}